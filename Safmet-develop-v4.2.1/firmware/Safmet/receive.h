#include <LowPower.h>

/** @file receive.h
 *  @brief This file contains definitions for all functions related to receiving transmissions

    ----------------------------------------------------------------------------------------------------------------
    Finder Mode: Broadcast and ACK

    Beacon in finder mode -> sends a broadcast and waits for ack response from eligible anchor nodes.   
    Once out of finder mode -> beacon will send poll message to first anchor node that has responded and wait for Poll Ack  
    it will finish communication with other anchor nodes sequentially   
    After finishing all the nodes (either range report or timeout), it will go   
    out of beacon mode(sleep or into anchor mode) and restart with a broadcast after timer expiry   
*/


/** @brief Check if the received broadcast message has the same NET_ID
 *  @startuml{check_for_nid} "Activity Diagram" width=5cm
 *  start
 *  if (network == device.Net_ID) then (yes);
 *    :Print "net id matched";
 *    :return true;
 *    stop
 *  endif
 *  :attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
 *  :delay(20);
 *  :Print "Nid_Not_M";
 *  :return false;
 *  stop
 *  @enduml */
bool check_for_nid() {
  //Serial.print(F("network : "));Serial.println(network);
  //Serial.print("device.Net_ID");Serial.println(device.Net_ID);
  if (network == device.Net_ID) {
    Serial.println(F("net id matched"));
    return true;
  }
  attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
  //delay(20);
  Serial.println(F("Nid_Not_M"));
  return false;
}

/** @brief Check if received data belongs to Ignore_UIDs
 *  @startuml{check_for_ignore_uid.png} "Activity Diagram" width=5cm
 *  :i=0;
 *  start
 *  repeat;
 *    if (source_UID==I_UD.Ignore_UID[i]) then (yes);
 *      :Print "ignore uid present : <source_UID";
 *      :Store timestamp in green_led_timer;
 *      :attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
 *      :receiver();
 *      :return false;
 *      break
 *    endif
 *  backward:i++;
 *  repeat while(i<3)
 *  stop
 *  @endumlreceived_packet */
bool check_for_ignore_uid() {
  for (int i = 0; i < 4; i++) {
    if (source_UID == I_UD.Ignore_UID[i]) {
      Serial.print(F("ignore uid present : ")); Serial.println(source_UID, HEX);
      green_led_timer = millis();
      attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
      receiver();
      return false;
    }
  }
  return true;
}

/** @brief Checks RX Data SubNet ID in #dist
 *  @startuml{check_for_sid.png} "Activity Diagram" width=5cm
 *  start
 *  :x=0;
 *  repeat
 *    if (def DEBUG) then (yes);
 *      :Print <x>;
 *      :Print dist_para.dist_SID[x];
 *      :Print "sub_network : sub_network";
 *    endif
 *    if (sub_network == dist_para.dist_SID[x] >> 16) then (yes);
 *      :sid_index=x;
 *      :return true;
 *      stop
 *    endif
 *  backward:x++;
 *  repeat while (x < sizeof(dist) / 4) then (yes);
 *  :Print "No Sid registered";
 *  :sleep_counter_led++;
 *  :delay(20);
 *  :receiver();
 *  :return false;
 *  stop
 *  @enduml */
bool check_for_sid()
{
  for (int x = 0; x < (sizeof(dist) / 4); x++) // dist_para strcut holding list of sub net
  {
#ifdef DEBUG
    Serial.print(x);
    Serial.println(dist_para.dist_SID[x], HEX);
    Serial.print(F("sub_network : ")); Serial.println(sub_network, HEX);
#endif
    if (sub_network == (dist_para.dist_SID[x] >> 16))
    {
      sid_index = x; // store the matched sid index in global varible to compare the respective distance in range packet
      //Serial.print("sid_index : ");Serial.println(sid_index);
      return true;
    }
  }
  Serial.println(F("No Sid registered"));

  sleep_counter_led++;

  //delay(20);
  attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
  receiver();
  return false;
}

/** @brief Puts the DW1000 in RX Mode
 *  @startuml{receiver.png} "Activity Diagram" width=5cm
 *  start
 *  :DW1000.newReceive();
 *  :DW1000.setDefaults();
 *  :DW1000.receivePermanently(true);
 *  :DW1000.startReceive();
 *  end
 *  @enduml */
void receiver() {
  DW1000.newReceive();
  DW1000.setDefaults();
  // so we don't need to restart the receiver manually
  DW1000.receivePermanently(true);
  DW1000.startReceive();
}

/** @brief Puts anchor in RX Mode for POLL */
void  resetInactive() {
  // anchor listens for POLL
  receiver();
}

/** @brief Store the received_source_UID in #source_uids_array */
void  save_src_in_array(uint32_t rec_src_UID) {
  response_count++;
  source_uids_array[response_count - 1] = rec_src_UID;
  /*
  AckStore_Time = micros() - AckStore_Time;
  Serial.print("AckStore Time = ");
  Serial.print(AckStore_Time);
  Serial.println(" us");
  */
}

/** @brief Check if Device Source_UID #source_UID in #source_uids_array
 *  if (source_uids_array[response_count] == cmp_src_uid) then (yes);
 *    :return true;
 *    stop
 *  endif
 *  :attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
 *  :receiver();
 *  :return false;
 *  stop
 *  @enduml */
bool  check_src_uid(uint32_t cmp_src_uid) {
  if ((source_uids_array[response_count]) == cmp_src_uid )    return true;
  attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
  receiver();
  return false;
}

/** @brief Check is RX Data Received Destination ID is same as device.Unique_ID
 *  @param[in] rec_des_uid Received Destination UID
 *  @startuml{check_dest_uid.png} "Activity Diagram" width=5cm
 *  start
 *  if (rec_des_uid == device_ID) then (yes);
 *    :attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
 *    :receiver();
 *    :return false;
 *  endif
 *  stop
 *  @enduml */
bool check_dest_uid(uint32_t rec_des_uid) {
  // if (rec_des_uid == device.Unique_ID) return true;
  if (rec_des_uid == getUID(device_ID)) return true;//csk
  attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
  receiver();
  return false;
}

/** @brief Transmits POLLACK upon receiving POLL
 *  @startuml{handlePollRx.png} "Activity Diagram" width=5cm
 *  start
 *  if (check_dest_uid(destination_UID)) then (no);
 *    stop
 *  endif
 *  :current_active_src_uid = source_UID;
 *  :timePollReceived = rx_time;
 *  :Store system timestamp in `listenModeActiveTime`;
 *  :transmit(POLLACK, packt_id, source_UID, payload, 0);
 *  stop
 *  @enduml */
void handlePollRx() {
  if (!check_dest_uid(destination_UID)) return;
  current_active_src_uid = source_UID;
  timePollReceived = rx_time;
  listenModeActiveTime = millis();
  transmit(POLLACK, packt_id, source_UID, payload, 0);
}

/** @brief Obtain Net_ID and Sub_Net_ID and check them. Return if any check is True else  
  * transmit acknowledgement
  * @startuml{handleBroadcastRx.png} "Activity Diagram" width=5cm
  * start
  * :network = (data[1] << 8) | data[2];
  * :sub_network = (data[3] << 8) | data[4];
  * if (check_for_nid()) then (no);
  *   stop
  * endif
  * if (check_for_ignore_uid()) then (yes);
  *   stop
  * endif
  * if (check_for_sid()) then (yes);
  *   stop
  * endif
  * :Store timestamp in `listenModeActiveTime`;
  * :transmit(ACK, packt_id, source_UID, payload, 0);
  * stop
  * @enduml */
void handleBroadcastRx() {
  //Serial.println(F("BroadCast received"));
  network = (data[1] << 8) | data[2];
  sub_network = (data[3] << 8) | data[4];
  if (!check_for_nid()) return;
  if (!check_for_ignore_uid()) return;
  if (!check_for_sid()) return;
  listenModeActiveTime = millis();
  //delay(random(0,listen_time/20) * 35);
  transmit(ACK, packt_id, source_UID, payload, 0);
}

/** @brief Checks if RX Data is for Beacon POLL and stores upto 3 IDs in source_uids_array */
void handleAckRx() { 
  Serial.print(F("GOTACK from "));
  Serial.println(source_UID);
  if (!check_dest_uid(destination_UID))  return;
  if (response_count <= max_process_size) save_src_in_array(source_UID); // during finer_mode if device response is more than "3"(currently max_process_size = 3) then it rejects any other response
}

//Data received handler function for DW1000
/** @brief Changes `receivedAck` to true */
void handleReceived() 
{

  receivedAck = true; //Set the received ack
  
  if(!RangeProcess_Flag) //if not in ranging process
  {      
    //AckStore_Time = micros();
    DW1000.getData(data, LEN_DATA);
    DW1000.getReceiveTimestamp(rx_time);
    received_packet_type = data[13];
  
    //To handle acks faster - Handle only the ACK and leave other packets
    if(received_packet_type == ACK) // If packet is ACK packet, then process it
    {
        receivedAck = false;
        CheckFlag = false;
        source_UID = getUID(data + 5);
        destination_UID = getUID(data + 9);
        handleAckRx();
    }
  }
  //Set the receivedAck flag and leave if in ranging process
  //Leave the other packet types to the beacon_receive_func();
}

/** @brief Send Range data after checking RX Data Source UID and Destination UID 
 *  @startuml{handlePollAckRx} "Activity Diagram" width=5cm
 *  if (check_src_uid(source_UID)) then (no);
 *    stop
 *  endif
 *  if (check_dest_uid(destination_UID)) then (no);
 *    stop
 *  endif
 *  :Store Received data timestamp in `timePollAckReceived`;
 *  :Store timestamp in listenModeActiveTime;
 *  :Send Range :: sendRange();
 *  stop
 *  @enduml */
void handlePollAckRx() {
  Serial.println(F("GOTPOLLACK"));
  if (!check_src_uid(source_UID)) return;
  if (!check_dest_uid(destination_UID)) return;

  timePollAckReceived = rx_time;
  listenModeActiveTime = millis();
  sendRange();
}

/** @brief Function obtains range from RX Data and stores in #distance
 *  @startuml{handleRangeReportRx.png} "Activity Diagram" width=5cm
 *  start
 *  if (check_src_uid(source_UID)) then (no);
 *    :return;
 *    stop
 *  endif
 *  if (!check_dest_uid(destination_UID)) then (no);
 *    :return;
 *    stop
 *  endif
 *  :Store Range in #curRange;
 *  if (curRange < 8) then (yes);
 *    :Turn VIB_MOTOR ON;
 *  endif
 *  :Turn GREEN_LED ON;
 *  :Print "Range_rep from : <dist_para.dist_SID[sid_index] >> 16><curRange>";
 *  :distance = curRange;
 *  stop
 *  @enduml */
void handleRangeReportRx() {
  
  //Serial.print(F("range:"));
  if (!check_src_uid(source_UID)) return;
  if (!check_dest_uid(destination_UID)) return;
  Serial.println("Got RangeReport");
  float curRange;
  memcpy(&curRange, data + 17, 4);

  if (curRange < 8)
  {
    /*
     * To Control the vibration motor please commment the below file or make the HIGH --> LOW or vice-versa
     */
    if (vib_motor == 1)
      digitalWrite(VIB_MOTOR, HIGH); // vibration motor//csk
          if (buzz == 1)
      analogWrite(BUZZER, 63);
  }

  //Blinking LED and Displaying Range Report
  digitalWrite(GREEN_LED, HIGH);
  //delay(100);

  Serial.println(F("Range_rep from : ")); Serial.print((dist_para.dist_SID[sid_index] >> 16), HEX); Serial.print(curRange); // for helmet there won't be any dist param so the RR from SID might be wrong
  distance = curRange;
  digitalWrite(GREEN_LED, LOW);
}

/** @brief Switches beacon to appropriate response upon receiving data
 *  @startuml{beacon_receive_func} "Activity Diagram" width=5cm
 *  start
 *  if (received_packet_type == ACK) then (yes);
 *    :handleAckRx()
 *    stop
 *  elseif (received_packet_type == POLLACK) then (yes);
 *    :handlePollAckRx()
 *    stop
 *  elseif (received_packet_type == RANGEREPORT) then (yes);
 *    :handleRangeReportRx()
 *    stop
 *  else (default);
 *    :default: attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
 *    stop
 *  endif
 *  stop
 *  @enduml */
void beacon_receive_func() {
  switch (received_packet_type) {
    /* ACK handle function is called inside ReceiveHandler of DW1000 before TWR starts
    case ACK:
    {
      handleAckRx(); 
      break;
    }*/
    case POLLACK: handlePollAckRx(); break;
    case RANGEREPORT:
    {
      handleRangeReportRx();
      //RangeProcess_Lock = false; //release the lock, to start the ranging process for next CASnode
      //Printing out the time required for completion of individual CASnodes
      RangingTime = millis() - RangingTime;

      Serial.println();
      Serial.print("Time required for CASnode ID ");
      Serial.print(current_active_src_uid);
      Serial.print(" : ");
      Serial.print(RangingTime);
      Serial.println(" ms");
      
      TotalRangingTime += RangingTime;
      if(SleepFlag == true)
      {
        
        CheckFlag = true;
        RangeProcess_Flag = false;
        SleepFlag = false;
        Serial.print("Total Ranging Time: ");
        Serial.print(TotalRangingTime);
        Serial.println(" ms");
        TotalRangingTime = 0; //Clear the variable for the next ranging process
        Serial.println();
        Serial.println("Process Complete Sleep");
        delay(10); //generic delay for the serial prints to complete before going to sleep - less than 10ms gives improper characters and the testscript fails decoding it
      
        //Atmega and DW1000 Sleep 
        DW1000.deepSleep();

        /*
        LowPower.idle(SLEEP_1S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
                    SPI_OFF, USART0_OFF, TWI_OFF);
        */

        uint32_t curtime = millis();
        if(curtime - lastBeaconTime >250)LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
        else if(curtime-lastBeaconTime>120) LowPower.powerDown(SLEEP_120MS, ADC_OFF, BOD_OFF);
        else if(curtime-lastBeaconTime>60) LowPower.powerDown(SLEEP_60MS, ADC_OFF, BOD_OFF);
        else if(curtime-lastBeaconTime>30) LowPower.powerDown(SLEEP_30MS, ADC_OFF, BOD_OFF);
        else if(curtime-lastBeaconTime>15) LowPower.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);
        
        //LowPower.powerDown(SLEEP_60MS, ADC_OFF, BOD_OFF);
                  
        DW1000.spiWakeup();

        decawaveInit();
        sendBeaconPacket();
        //Process Complete Sleep
        
      }
      break;
    }
    default:
    {
      attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING); 
      //Serial.println("Default Packet Type");
      break;
    }
  }
}

/** @brief Obtain packet type, source_UID and destination_UID from received packet, then execute beacon_receive_func
 *  @startuml{received_packet.png} "Activity Diagram" width=5cm
 *  start
 *  :Store received data on DW1000 :: DW1000.getData(data,LEN_DATA);
 *  :Obtain ReceiveTimestamp :: DW1000.getReceiveTimestamp(rx_time);
 *  :Store received packet type in received_packet_type;
 *  if(def DEBUG) then (yes);
 *    :Print "rece pa : <received_packet_type>";
 *  endif
 *  :Get Source UID;
 *  if(def DEBUG) then (yes);
 *    :Print "source uid : <source_UID>";
 *  endif
 *  :Get Destination UID;
 *  if(def DEBUG) then (yes);
 *    :Print "dest uid : <destination_UID>";
 *  endif
 *  :beacon_receive_func();
 *  stop
 *  @enduml */
void received_packet() //need to check
{
  DW1000.getData(data, LEN_DATA);
  DW1000.getReceiveTimestamp(rx_time);
  received_packet_type = data[13];
  if(received_packet_type == ACK) //As ACKs are handled in the handler function 
  {
    return; //Only ACKs are allowed outside of ranging process i.e inside the handler function's if logic
  }
#ifdef DEBUG
  Serial.print(F("rece pa : ")); Serial.println(data[13]);
#endif

  /***********************************************************************/
  source_UID = getUID(data + 5); //Fetching the source_UID from ACK packet 
  /***********************************************************************/
  
#ifdef DEBUG
  Serial.print(F("source uid : ")); Serial.println(source_UID, HEX); //FROM
#endif
  
  /*********************************************************************************/
  destination_UID = getUID(data + 9); //Fetching the destination_UID from ACK packet
  /*********************************************************************************/
  
#ifdef DEBUG
  Serial.print(F("dest uid : ")); Serial.println(destination_UID, HEX); //TO
#endif
  //If beacon mode enabled
  beacon_receive_func();
}

/** @file transmit.h
 *  @brief Contains functions related to data transmission using DW1000 */

#include <avr/wdt.h>


byte send_data[LEN_DATA];
void receiver();

/** @brief Fill default values in #send_data array when transmitting 
 *  Store Net_ID in first 2 bytes   
 *  Store Sub_Net_ID in next 2 bytes  
 *  Store device_ID in last 4 bytes */
void fill_default_transmit() {
  send_data[1] = device.Net_ID >> 8;           //1 msb
  send_data[2] = device.Net_ID;                //2 lsb
  send_data[3] = device.Sub_Net_ID >> 8;       //3
  send_data[4] = device.Sub_Net_ID ;              //4
  send_data[5] = device_ID[0];//csk
  send_data[6] = device_ID[1];//csk
  send_data[7] = device_ID[2];//csk
  send_data[8] = device_ID[3];//csk
}

/** @brief DW1000 transmits data as specified in arguments
 *  Function uses the global variables #timeRangeSent and #send_data
 *  @param[in] packt_type       Type of Packet being sent
 *  @param[in] pack_id          ID of packet being sent
 *  @param[in] dest_id          ID of device to which packet is being sent
 *  @param[in] payload[]        Variable with data being sent
 *  @param[in] payload_size     Size of #payload
 *  @startuml{transmit.png} "Activity Diagram for function" width=5cm
 *  start
 *  :Store data from input arguments in send_data;
 *  :attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
 *  :yield();
 *  :Open a new transmit session :: DW1000.newTransmit(); 
 *  :Set Default configuration for DW1000 :: DW1000.setDefaults();
 *  if (packt_type == RANGE) then (yes);
 *    :Compute delay time and store in deltaTime;
 *    :Set the future timestamp of DW1000 packet going to be sent and also store it in timeRangeSent;
 *    :Store the computed future timestamp as a byte array in send_data;
 *  endif
 *  if (packt_type == POLLACK) then (yes);
 *    :Compute delay time and store in deltaTime;
 *    :Set the future timestamp of DW1000 packet going to be sent;
 *  endif
 *  :Set the array data from send_data on DW1000;
 *  :Finally start the transmission;
 *  end
 *  @enduml */
void transmit(uint8_t packt_type, uint8_t pack_id, uint32_t dest_id, uint8_t payload[], uint8_t payload_size) //Build the packet, then setup and transmit using DW1000
{
  send_data[0] = pack_id;
  sent_packet_type = packt_type;
  send_data[1] = device.Net_ID >> 8;           //1 msb
  send_data[2] = device.Net_ID;                //2 lsb
  send_data[3] = device.Sub_Net_ID >> 8;       //3
  send_data[4] = device.Sub_Net_ID;              //4
  send_data[9] = dest_id >> 24;//0xFA              //9
  send_data[10] = dest_id >> 16;//0xFB             //10
  send_data[11] = dest_id >> 8;//0xFC              //11
  send_data[12] = dest_id; //0xFD                  //12
  send_data[13] = packt_type;                     //13

  send_data[14] = payload_size + 2;                 //14

  send_data[15] = packt_type;                       //15
  send_data[16] = payload_size;                     //16
  for (int i = 0; i < payload_size; i++)  // payload starts from data[15]
  {
    send_data[17 + i] = payload[i];
  }

  attachInterrupt(digitalPinToInterrupt(PIN_IRQ), DW1000Class::handleInterrupt, RISING);
  yield();
  DW1000.newTransmit();
  DW1000.setDefaults();
  if (packt_type == RANGE) {
    DW1000Time deltaTime = DW1000Time(replyDelayTimeUS, DW1000Time::MICROSECONDS);
    timeRangeSent = DW1000.setDelay(deltaTime);
    timeRangeSent.getTimestamp(send_data + 27);
  }
  else if (packt_type == POLLACK)
  {
    DW1000Time deltaTime = DW1000Time(replyDelayTimeUS, DW1000Time::MICROSECONDS);
    DW1000.setDelay(deltaTime);
  }
  DW1000.setData(send_data, LEN_DATA);
  DW1000.startTransmit();
}

/** @brief Changes #sentAck to `True`
 *  Interrupt attached to DW1000 to stop sending a message repeatedly */ 
void handleSent()
{
  sentAck = true;
}

/** @brief This function computes the time of flight and distance
 *  The function uses the equation from Asymmetric Double Sided TWR(Two Way Ranging)  
 *  It uses the variables #timePollSent, #timePollReceived, #timePollAckSent, #timePollAckReceived, #timeRangeSent, #timeRangeReceived   
 *  For more details check the chapter in the manual : https://www.decawave.com/sites/default/files/resources/dw1000_user_manual_2.11.pdf
 *  @startuml{computeRangeAsymmetric.png} "Computation of Range in Safmet" width=5cm
 *  start
 *  :round1 = (timePollAckReceived-timePollSent); 
 *  :reply1 = (timePollAckSent - timePollReceived); 
 *  :round2 = (timeRangeReceived - timePollAckSent); 
 *  :reply2 = (timeRangeSent - timePollAckReceived);
 *  :tof = (round1 * round2 - reply1 * reply2) / (round1 + round2 + reply1 + reply2);
 *  :Compute range :: timeComputedRange.setTimestamp(tof);
 *  end
 *  @enduml*/
void computeRangeAsymmetric()
{
  DW1000Time round1 = (timePollAckReceived - timePollSent).wrap();
  //Serial.print(","); Serial.println(round1);
  DW1000Time reply1 = (timePollAckSent - timePollReceived).wrap();
  //Serial.print(","); Serial.println(reply1);
  DW1000Time round2 = (timeRangeReceived - timePollAckSent).wrap();
  // Serial.print(","); Serial.println(round2);
  DW1000Time reply2 = (timeRangeSent - timePollAckReceived).wrap();
  // Serial.print(","); Serial.println(reply2);
  DW1000Time tof = (round1 * round2 - reply1 * reply2) / (round1 + round2 + reply1 + reply2);
  // Serial.print(","); Serial.println(tof);
  timeComputedRange.setTimestamp(tof);
}

/** @brief Send Range data upon receiving POLL Acknowledgement 
 *  @startuml{sendRange.png} "Activity Diagram for sendRange()" width=5cm
 *  start
 *  :Print "sent range";
 *  :Turn GREEN_LED ON;
 *  :Set sleep_counter_led to 0;
 *  :Reset the watchdog;
 *  :Obtain System Timestamp;
 *  :Store timePollSent as byte array in payload;
 *  :Store timePollAckReceived as byte array in payload[5:];
 *  :Store timeRangeSent as byte array in payload[10:];
 *  :Perform a Range transmission;
 *  end
 *  @enduml*/
void sendRange()
{
  //DW1000.getReceiveTimestamp(timePollAckReceived);
  //Serial.println(F("sent range"));

  digitalWrite(GREEN_LED, HIGH);
//  pinMode(BUZZER, OUTPUT);
//  tone(BUZZER, 4000);//commented beacause safmet resetting and showing low battery at 3.5v bat
  //digitalWrite(BUZZER, HIGH);
  sleep_counter_led = 0;

  wdt_reset();

  //wdt_feed_1 = millis();
  /// Why is `DW1000.getSystemTimestamp(timeRangeSent) performed ???
  DW1000.getSystemTimestamp(timeRangeSent);//setDelay(deltaTime); // do not remove this function. if you remove this then you won't get the range
  timePollSent.getTimestamp(payload);
  //Serial.print(F("timePollSent")); Serial.println(timePollSent);
  timePollAckReceived.getTimestamp(payload + 5);
  //Serial.print(F("timePollAckReceived")); Serial.println(timePollAckReceived);
  timeRangeSent.getTimestamp(payload + 10);
  //Serial.print(F("timeRangeSent")); Serial.println(timeRangeSent);
  transmit(RANGE, packt_id, source_uids_array[response_count], payload, 15);
  Serial.println("Range Sent");
}

/** @brief Perform certain common functional operations based on Packet Type being transmitted
 *  @param[in] snt_pkt Byte specifying type of packet
 *  @startuml{sent_packet.png} "Activity Diagram for sent_packet" width=10cm
 *  start
 *  if (snt_pkt?) then (BROADCAST)
 *    :noteActivity();
 *  elseif (ACK) 
 *    :noteActivity();
 *  elseif (POLL)
 *    :get DW1000 Transmission Timestamp from timePollSent;
 *    :noteActivity();
 *  elseif (POLLACK)
 *    :get DW1000 Transmission Timestamp from timePollAckSent;
 *    :noteActivity();
 *  elseif (RANGE)
 *    :get DW1000 Transmission Timestamp from timeRangeSent;
 *    :noteActivity();
 *  elseif (RANGEREPORT)
 *  endif
 *  stop
 *  @enduml */ 
void sent_packet(byte snt_pkt)
{
  //Serial.print(F("sent packet : ")); Serial.println(snt_pkt);
  switch (snt_pkt)
  {
    case BROADCAST: noteActivity(); break;
    case ACK: noteActivity(); break;
    case POLL: DW1000.getTransmitTimestamp(timePollSent); noteActivity(); break;
    case POLLACK: DW1000.getTransmitTimestamp(timePollAckSent); noteActivity(); break;
    case RANGE: DW1000.getTransmitTimestamp(timeRangeSent); noteActivity();  break;
    case RANGEREPORT: break;
  }
}

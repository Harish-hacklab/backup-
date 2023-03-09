/** @file LoopFunctions.h
    @brief This file contains various functions related to Data sending/receiving,
    beacon mode, touch sensor, led notification */

/** @brief Respond to received data
    @startuml{handleReceivedData.png} "Activity Diagram" width=10cm
    start
    if (receivedAck and !sleep_act) then (yes)
      :Reset Watchdog;
      :`receivedAck`=false;
      :Print "RXM"
      :delay(random(0,10));
      :received_packet();
    endif
    end
    @enduml */
void handleReceivedData() {
  if (receivedAck && !sleep_act)
  {
    wdt_reset();
    //CheckFlag = false;
    receivedAck = false;
    Serial.println(F("RXM"));
    //delay(random(0, 10));
    delay(1);
    received_packet();
  }
}

/** @brief Function handler in TX Mode
    @startuml{handleSentData.png} "Activity Diagram" width=5cm
    start
    if (sentAck and !sleep_act) then (yes);
      :if (enter) then (yes);
      :sleep_test--;
      :enter=false;
    endif
    :counter=0;
    :Store system timestamp in listenModeActiveTime;
    :Reset Watchdog;
    :sentAck=false;
    :Print "TXM";
    :all_ok=true;
    :sent_packet(sent_packet_type);
    stop
    @enduml */
void handleSentData() {
  if (sentAck && !sleep_act)
  {
    if (enter)
    {
      sleep_test--;
      enter = false;
    }
    counter = 0;
    listenModeActiveTime = millis(); // added
    wdt_reset();
    sentAck = false;
    Serial.println(F("TXM"));
    all_ok = true;
    sent_packet(sent_packet_type);
  }
}

#if TOUCH_ENABLE
/** @brief Boolean that reads state of Touch Sensor
    @startuml{checkTouchEnable.png} "Activity Diagram" width=5cm
    start
    if (digitalRead(TOUCH_SENSOR)) then (no);
      :Print "No touch";
      :Reset Watchdog;
      :delay(200);
      :return false;
      stop
    endif
    return true;
    stop
    @enduml */
bool checkTouchEnable() {
  if (!digitalRead(TOUCH_SENSOR))
  {
    Serial.println(F("No touch"));
    wdt_reset();
    delay(20);
    return false;
  }
  return true;
}
#endif




/** @brief Device transmits BROADCAST or POLL based on global variables
    @startuml{sendBeacon.png} "Activity Diagram" width=10cm
    start
    if (millis() - lastBeaconTime > param.beacon_timeout) then (yes);
      :gpio_defaults();
      if (response_count <= 0) then (yes);
        if (def TOUCH_ENABLE) then (yes);
          if (checkTouchEnable()) then (no);
            :return;
            stop
          endif
        endif
        :packt_id++;
        :Print "BEACON";
        :delay(random(0,50));
        :transmit(BROADCAST, packt_id, broadcast_address, payload, 0);
        :Store system timestamp in lastBeaconTime;
        :Store system timestamp in listenModeActiveTime;
      endif
    endif
    if ((!receivedAck && !sentAck) && (millis() - listenModeActiveTime > listen_time) && (response_count > 0));
      :transmit(POLL, packt_id, source_uids_array[response_count - 1], payload, 0);
      :Store system time in listenModeActiveTime;
      :response_count--;
    endif
    stop
    @enduml */
void sendBeacon() {
  if (millis() - lastBeaconTime > param.beacon_timeout) //Beacon Timeout
  {
    sendBeaconPacket();
  }
  else if ((!receivedAck && !sentAck) && (millis() - listenModeActiveTime > listen_time) && (response_count > 0))// && !RangeProcess_Lock)
  {
    //RangeProcess_Lock = true; //Set the Lock for the current CASnode
    RangeProcess_Flag = true;// Start the ranging process
    RangingTime = millis(); //Ranging Process Init - POLL message - Ranging Time init
    transmit(POLL, packt_id, source_uids_array[response_count - 1], payload, 0);
    current_active_src_uid = source_uids_array[response_count - 1];
    Serial.println(F("POLL Sent"));
    listenModeActiveTime = millis();
    Serial.print(F("Response Count Value: "));
    Serial.println(response_count);
    response_count--;
    if (response_count == 0)
    {
      SleepFlag = true;
    }
  }
  else if (millis() - lastBeaconTime > BoradcastTimeout_Time && !RangeProcess_Flag) //check for 30ms and see if the range process is going on, then go to broadcast timeout sleep
  {
    //Atmega and DW1000 Sleep
    Serial.println(F("Broadcast Timeout Sleep"));
    DW1000.deepSleep();
    delay(5); //Generic delay for the serial print
    /*
      LowPower.idle(SLEEP_250MS, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF,
                SPI_OFF, USART0_OFF, TWI_OFF); //250MS Sleep Atmega and DW1000
    */

    //Sleep for 265MS as supported by the LowPower Library as the remaining 35 ms: 30 for broadcast timeout sleep and 5 for generic delay, total 300ms (after which next broadcast will be sent)
    //Adjust 5ms in listen time
    uint32_t curtime = millis();
    if(curtime - lastBeaconTime >250)LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);
    else if(curtime-lastBeaconTime>120) LowPower.powerDown(SLEEP_120MS, ADC_OFF, BOD_OFF);
    else if(curtime-lastBeaconTime>60) LowPower.powerDown(SLEEP_60MS, ADC_OFF, BOD_OFF);
    else if(curtime-lastBeaconTime>30) LowPower.powerDown(SLEEP_30MS, ADC_OFF, BOD_OFF);
    else if(curtime-lastBeaconTime>15) LowPower.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);
    //LowPower.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF);

    Serial.println(F("Waking up from sleep"));
    DW1000.spiWakeup();
    decawaveInit();
    //Broadcast Timeout Sleep
    sendBeaconPacket();
  }
}

/** @brief Resets sleep_counter_led if it exceeds 20 and checks Battery Voltage
    @startuml{awake_led_notification.png} "Activity Diagram" width=5cm
    if (sleep_counter_led > 20) then (yes);
      :sleep_counter_led=0;
      :Turn GREEN_LED ON;
      :delay(50);
      :Print Battery Voltage;
      if (bat_voltage <= BAT_LOW_ALERT) then (yes);
        :Turn GREEN_LED ON;
      else (no);
        :Turn GREEN_LED OFF;
      endif
    endif
    stop
    @enduml */
void awake_led_notification() {
  if (sleep_counter_led > 20)
  {
    sleep_counter_led = 0;
    digitalWrite(GREEN_LED , HIGH);
    delay(50);
    Serial.print(F("\nBattery Voltage 1 :: ")); Serial.println(bat_voltage);

    if (bat_voltage <= BAT_LOW_ALERT)
      digitalWrite(GREEN_LED, HIGH);
    else
      digitalWrite(GREEN_LED , LOW);
  }
}
/*
  void SerialHandle()
  {
  while (Serial.available())
  {
      InputFlag = true;
      char c = Serial.read();
      if(c == '\n')//Skip the newline character
      {
        continue;
      }
      SerialInput += c;
  }
  Serial.print("Serial Input: ");
  Serial.println(SerialInput);
  delay(100); //100ms delay
  if (InputFlag == true)
  {
    InputFlag = false;
    if (SerialInput == "DeviceName")
    {
      for(int i=0;i<4;i++)
      {
        Serial.print(dev_ID[i]);
        Serial.print(" ");
      }
      Serial.println();
    }
    SerialInput = "";
  }
  }*/

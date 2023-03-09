/**
    @file Safmet.ino
    @brief This file contains the main setup and loop.
*/
#include <avr/wdt.h> //avr Watchdogtimer header
#include "SPI.h"
#include "DW1000.h"

#include "dev_id.h"
#include "HSA.h"
#include "common.h"
#include "transmit.h"
#include "receive.h"
#include "SetupInit.h"
#include "LoopFunctions.h"

#include <LowPower.h> //low power library for atmega328p
/** @brief This function runs at the start and only once. It sets up pins, serial, decawave, watchdog
    and prints the Flash Settings
    @startuml{setup.png} "Setup" width=5cm
    start
    :pinSetup();
    :serialSetup();
    :Enable Watchdog :: wdt_enable(WDTO_2S);
    :Check Battery voltage :: check_vbat();
    :Check for Input Power :: check_power();
    :Reset the Watchdog :: wdt_reset();
    :Initialize Decawave :: decawaveInit();
    :setupDeviceSettings();
    :delay(50);
    group Attach Interrupts for Decawave
      :DW1000.attachSentHandler(handleSent);
      :DW1000.attachReceivedHandler(handleReceived);
    endgroup
    :Initialize Decawave receiver :: receiver();
    :Reset the Watchdog :: wdt_reset();
    :fill_default_transmit();
    :Print Settings of Device stored in Flash :: printFlashSettings();
    end
    @enduml */
void setup()
{
  /** Calls the following functions :-
      1. #pinSetup()
      2. #serialSetup()
      3. #check_vbat()
      4. #check_power
      5. #decawaveInit()
      6. #setupDeviceSettings()
      7. #receiver()
      8. #fill_default_transmit
      9. #printFlashSettings() */
  pinSetup();
  serialSetup();
  wdt_enable(WDTO_2S);

  check_vbat();
  check_power();
  wdt_reset();
  //wdt_disable();    //why disabling wdt?????
  decawaveInit();
  setupDeviceSettings();
  //delay(50);

  DW1000.attachSentHandler(handleSent);
  DW1000.attachReceivedHandler(handleReceived);

  receiver();
  wdt_reset();
  fill_default_transmit();
  sendBeaconPacket();
  //  printFlashSettings();// comment to print data in  Printing data in parameter

  //  if (bat_voltage <= BAT_LOW_ALERT)
  //    digitalWrite(GREEN_LED, HIGH);
  //  else
  //    digitalWrite(GREEN_LED, LOW);
  if((millis()- green_init)>20){
    digitalWrite(GREEN_LED, LOW);
    green_init = millis();
  }
}
/* send broacast and enable finder_mode to collect the available unique devices till the listen_time expires.
  once the broadcast has sent, listen for some time by enabling finder mode.\
  after the listen time expires disable the finder mode and respond for the collected UID from the above mentioned period.
  if suppose no available devices present at the listen period send broadcast.*/

/** @brief This runs continuously after setup
    @startuml{loop.png} "Activity Diagram" width=5cm
    :awake_led_notification();
    :check_power();
    :check_vbat();
    :handleReceivedData();
    :handleSentData();
    :sendBeacon();
    @enduml */
void loop()
{
  if (CheckFlag) //Skip if Ranging Process is going on
  {
    awake_led_notification();
    check_power();
    check_vbat();
  }
  handleReceivedData();
  handleSentData();
  sendBeacon();
  /*if(Serial.available())
    {
    SerialHandle();
    }*/
  //Giving alive Pulse to ATTINY to avoid resetting the system
  if (millis() - lastActivity > 10) {
    digitalWrite(ATTINY_GPIO, LOW);
  }
  //
}

/** @brief Reinitializes the DW1000
    @startuml{reInitDeca.png} "Activity Diagram" width=5cm
    start
    :Reset Watchdog;
    :Turn ATTINY_GPIO HIGH;
    :Turn ATTINY_GPIO LOW;
    :DW1000 wake up SPI :: DW1000.spiWakeup();
    :delay(2);
    :DW1000.begin(PIN,IRQ, PIN_RST);
    :DW1000.select(PIN_SS);
    :DW1000.newConfiguration();
    :DW1000.setDefaults();
    :DW1000.setDeviceAddress(1);
    :DW1000.setNetworkId(10);
    :DW1000.enableMode(DW1000.MODE_SHORTDATA_FAST_ACCURACY); //MODE_SHORTDATA_FAST_ACCURACY);
    :DW1000.commitConfiguration();
    :DW1000.attachSentHandler(handleSent);
    :DW1000.attachReceivedHandler(handleReceived);
    :receiver();
    :fill_default_transmit();
    stop
    @enduml */

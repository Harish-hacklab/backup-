/** @file common.h
 *  @brief File contains functions that are used to obtain UID, print flash settings and setup some gpios */

#include <avr/wdt.h>

/** @brief Turns #VIB_MOTOR Low and changes GREEN_LED status based on #bat_voltage */
void gpio_defaults()
{
   pinMode(BUZZER, OUTPUT);
  analogWrite(BUZZER, 0);
  digitalWrite(VIB_MOTOR, LOW);
  
  if (bat_voltage <= BAT_LOW_ALERT)
    digitalWrite(GREEN_LED, HIGH);
  else
    digitalWrite(GREEN_LED, LOW);
}

/** @brief update activity timestamp, so that we do not reach "resetPeriod" */
void noteActivity()
{
  lastActivity = millis();
  digitalWrite(ATTINY_GPIO, HIGH);
}

/** @brief Used to print the #data array content stored from received packets */
void debug_data()
{
  for (int i = 0; i < LEN_DATA; i++)
  {
    Serial.print(F("data_RX"));
    Serial.print(F("["));
    Serial.print(i);
    Serial.print(F("]"));
    Serial.println(data[i], HEX);
  }
}

/** @brief Clears the #payload */
void clear_payload()
{
  for (int i = 0; i < PAYLOADSIZE; i++)
    payload[i] = NULL;
}

/** @brief Obtains #Unique_ID of device from data for received packets */
uint32_t getUID(byte data[])
{
  uint32_t id = 0;
  for (int i = 0; i < 4; i++)
    id |= ((uint32_t)data[3 - i] << i * 8);
  return id;
}

/** @brief Prints all variable values stored in #setupDeviceSettings */
void  printFlashSettings()
{
  Serial.println(F("printing data in dev_id structure"));
  Serial.print(F("DEVICE UID :: "));
  Serial.print(device_ID[0], HEX);
  Serial.print(device_ID[1], HEX);
  Serial.print(device_ID[2], HEX);
  Serial.println(device_ID[3], HEX);
  Serial.print(F("device.Net_ID   :: "));
  Serial.println(device.Net_ID, HEX);
  Serial.print(F("device.Sub_Net_ID :: "));
  Serial.println(device.Sub_Net_ID, HEX);
  //  Serial.print(F("device.Unique_ID :: "));//csk
  //  Serial.println(device.Unique_ID, HEX);
 
  //--------------------------------
  Serial.println(F("Printing data in parameter"));
  Serial.print(F("param.en_func :: "));
  Serial.println(param.en_func, HEX);
  Serial.print(F("param.tx_pwr  :: "));
  Serial.println(param.tx_pwr, HEX);
  Serial.print(F("param.rx_pwr  :: "));
  Serial.println(param.rx_pwr, HEX);
  Serial.print(F("param.op_mode :: "));
  Serial.println(param.op_mode, HEX);
  Serial.print(F("param.sleep_time  :: "));
  Serial.println(param.sleep_time, HEX);
  Serial.print(F("param.wait_time  :: "));
  Serial.println(param.wait_time, HEX);
  Serial.print(F("param beacon_timeout :: "));
  Serial.println(param.beacon_timeout);
  //----------------------------------
  Serial.println(F("Printing Ignore_UIDs "));
  for (int i = 0; i < 8; i++)
  {
    Serial.print(F("Ignore_UID :: "));
    Serial.print(i); Serial.print(F(" :: "));
    Serial.println(I_UD.Ignore_UID[i], HEX);
  }
  //-----------------------------------------
  Serial.println(F("Printing SSIDs and Distances "));
  for (int i = 0; i < 5; i++)
  {
    Serial.print(F("DIST_SID :: "));
    Serial.print(i); Serial.print(F(" :: "));
    Serial.println((dist_para.dist_SID[i] & 0xFFFF0000) >> 16, HEX);
    Serial.print(F("Near_"));
    Serial.print(i); Serial.print(F(" :: "));
    Serial.println((dist_para.dist_SID[i] & 0x0000FF00) >> 8, HEX);
    Serial.print(F("Emergency_"));
    Serial.print(i); Serial.print(F(" :: "));
    Serial.println((dist_para.dist_SID[i] & 0x000000FF), HEX);
    //-------------------------------------------

    Serial.print(F("sleep_timeout:")); Serial.println(sleep_timeout);//csk
    Serial.print(F("temp_near:")); Serial.println(temp_near);//csk
    Serial.print(F("dist_n_f:")); Serial.println(dist_n_f);//csk
    Serial.print(F("dist_e_f:")); Serial.println(dist_e_f);//csk
    Serial.print(F("dist_n_f_f:")); Serial.println(dist_n_f_f);//csk
    Serial.print(F("dist_e_f_f:")); Serial.println(dist_e_f_f);//csk
  }
}

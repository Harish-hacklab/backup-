/** @file SetupInit.h
    @brief File contains all functions that initializes different components to run Safmet */
#include "git-version.h"

/** @brief Initialize all GPIOs
    1. 5 - OUTPUT - HIGH
    2. VIB_MOTOR - OUTPUT - HIGH
    3. ATTINY_GPIO - OUTPUT - LOW
    4. DW_LS - OUTPUT - HIGH
    5. GREEN_LED - OUTPUT - LOW
    6. BLUE_LED - OUTPUT - LOW
    7. TOUCH_SENSOR - INPUT
    8. T_PWR - OUTPUT
    9. if TOUCH_ENABLE then
       a. TOUCH_SENSOR - INPUT
       b. T_PWR - HIGH
    10. PWR_ADC - INPUT
    11. VBAT_ADC - INPUT */
void pinSetup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(BUZZER, OUTPUT);
  analogWrite(BUZZER, 0);
  pinMode(VIB_MOTOR, OUTPUT);
  digitalWrite(VIB_MOTOR, LOW);

  pinMode(ATTINY_GPIO, OUTPUT);
  //delay(500);
  digitalWrite(ATTINY_GPIO, LOW);
  pinMode(DW_LS, OUTPUT);

  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);
  green_setup();
  pinMode(BLUE_LED, OUTPUT);

  pinMode(TOUCH_SENSOR, INPUT);
  pinMode(T_PWR, OUTPUT);
  digitalWrite(DW_LS, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);

#if TOUCH_ENABLE
  pinMode(TOUCH_SENSOR, INPUT);
  digitalWrite(T_PWR, HIGH);
#endif

  digitalWrite(DW_LS, HIGH);
  pinMode(PWR_ADC, INPUT);
  pinMode(VBAT_ADC, INPUT);
}

/** @brief Initializes Serial and prints git tag details
    @startuml{serialSetup.png} "Activity Diagram for serialSetup()" width=5cm
    start
    :Initialize Serial at baudrate 115200;
    :Print Git Tag details in format `tag-numberOfCommitsAhead-gCommitID-dirty/clean`;
    end
    @enduml */
void serialSetup() {
  Serial.begin(115200); //use serial.begin only
  Serial.println(F("\n\n\n\nSAFMET")); //Serial.println(vershun);
  Serial.println(F("SAFMET BEGIN")); //Serial.println(vershun);
  Serial.println("Git Version : "); Serial.println(GIT_VERSION);
}

/** @brief Sets the state of GPIOs during charging mode inside #check_power
    @startuml{setGPIOsStateOnCharging.png} "Activity Diagram for setGPIOsStateOnCharging" width=5cm
    start
    :Set T_PWR to LOW;
    :Turn GREEN_LED OFF;
    :Set VIB_MOTOR to LOW;
    end
    @enduml */
void setGPIOsStateOnCharging() {
  digitalWrite(T_PWR, LOW);
  digitalWrite(GREEN_LED , LOW);
  pinMode(BUZZER, OUTPUT);
  analogWrite(BUZZER, 0);
  digitalWrite(VIB_MOTOR, LOW);
}

/** @brief Checks battery voltage, input power and charging status
    @startuml{check_power.png} "Activity Diagram" width=5cm
    start
    :Reset Watchdog;
    :Set analogReference to DEFAULT;
    :Set ChargingFlag and ChargedFlag to false;
    :Initialize VoltHistCount;
    repeat
      :ChargingFlag=true;
      :Reset Watchdog;
      :setGPIOsStateOnCharging();
      if (ChargedFlag) then (yes);
        :Turn GREEN_LED ON;
      endif
      if (millis() - printtime > 1 second) then (yes);
        :Print "power on";
        :Store system timestamp in printtime;
        :Set VBAT_ADC to INPUT;
        :Set analogReference to INTERNAL;
        :Calculate battery voltage from VBAT_ADC output;
        :Print Battery Voltage;
      endif
      if (millis() - VoltHistoryTime > 10000) then (yes);
        :Store system timestamp in VoltHistoryTime;
        :Set VBAT_ADC to INPUT;
        :Set analogReference to INTERNAL;
        :Calculate battery voltage;
        :Store Battery Voltage in VoltHist[VoltHistCount];
        :Print "<VoltHistCount>: Noting Voltage;
        :VoltHistCount = VoltHistCount + 1;
        if (VoltHistCount % 10 == 0) then (yes);
          :VoltHistCount=0;
          :ChargedFlag=true;
          :Run for loop where it is checked if latest reading in VoltHist exceeds 4V or is increasing;
          if (VoltHist is greater than 4V or increasing) then (yes);
            :ChargedFlag = false;
            :Print "Still Charging & Battery Voltage < 4V";
            stop
          endif
          :Reset Watchdog;
          if(ChargedFlag) then (no);
            :Turn GREEN_LED OFF;
            :Print "Charging!";
          else (yes)
            :Turn GREEN_LED ON;
            :Print Fully Charged, disconnect charger now;
          endif
        endif
      endif
    repeat while(analogRead(PWR_ADC) < 800)
    if (ChargingFlag) then (yes);
      :ChargingFlag=false;
      :Turn ATTINY_GPIO HIGH then LOW in an interval of 100ms once;
      :Turn GREEN_LED OFF;
    endif
    :Turn T_PWR HIGH;
    stop
    @enduml */
void check_power()
{
  wdt_reset();
  analogReference(DEFAULT);
  boolean ChargingFlag = false;
  boolean ChargedFlag = false;
  int VoltHistCount = 0;
  // Run while loop when connected to charger
  while (analogRead(PWR_ADC) > 800)//R1 , R2  :: Keep running while loop when receiving Power INPUT (this while loop gets executed while charging)
  {
    ChargingFlag = true;
    wdt_reset();// in order not to reset continuously while looping in while loop
    setGPIOsStateOnCharging();

    //Code to read Serial Input and Display Information during charging 
    /*
    while (Serial.available())
    {
      InputFlag = true;
      char c = Serial.read();
      if(c == '\n')//skip the newline character
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
    */
    
    if (ChargedFlag)
    {
      digitalWrite(GREEN_LED, HIGH);
    }
    if ((millis() - printtime > 1000))// checking the current time with the timer and also to make sure that this loop wont execute unlimitedly.
    {
      //checking input power voltage
      Serial.println(F("power on"));
      printtime = millis();// ensure that we initialise timer with current value, so that the condition in if doesnt satisfy repeatedly.

      pinMode(VBAT_ADC, INPUT);
      analogReference(INTERNAL);
      volt = analogRead(VBAT_ADC) * 1.1 / 1023.00; //(10K-VD_R1,3.9K-VD_R2)
      bat_voltage = (volt * (VD_R1 + VD_R2) / (VD_R2));
      Serial.print(F("\nBattery Voltage 2 :: ")); Serial.println(bat_voltage);
      Serial.print(F("\nBattery Voltage 2 :: ")); Serial.println(bat_voltage);

      digitalWrite(LED_IND3, LOW);
      analogWrite(LED_IND2, 0);
      analogWrite(LED_IND1, 0); 
        delay(500);
      digitalWrite(LED_IND3, HIGH);
      delay(500);
        analogWrite(LED_IND2, 255);
        delay(500);
        analogWrite(LED_IND1, 255); 
        delay(500);
        digitalWrite(GREEN_LED, HIGH);
        delay(100);

    }
    //Serial.println("POWER ON FOR CHARGING SAFMET");

//    //Send alive pulse to ATTINY
//    digitalWrite(ATTINY_GPIO, HIGH);
//    delay(10); //10ms delay
//    digitalWrite(ATTINY_GPIO, LOW);
    
  }
  // When disconnected from the charger check with the charging was true. If it was true then send signal to ATINY to reset
  // This is don to ensure that decawave is not stuck

  if (ChargingFlag) // this condition will never be `True`, if true then reset the system
  {
    Serial.println(F("SAFMET HAS BEEN REMOVED FROM CHARGE"));
    delay(3);
    ChargingFlag = false;
    if (ChargingFlag == false) {
//      digitalWrite(ATTINY_GPIO, HIGH);
//      delay(100);
      digitalWrite(ATTINY_GPIO, LOW);
      digitalWrite(GREEN_LED, LOW);
    }
  }
  digitalWrite(T_PWR, HIGH); // Activate touch sensor
}

/** @brief Calculates voltage in battery
    @startuml{check_vbat.png} "Activity diagram for check_vbat()" width=5cm
    start
    if (current time - last time Voltage was printed through function is greater than 1 second) then (yes);
      :Print "Check Bat Voltage";
      :Store current time in `printtime`;
      :Set analogReference as Internal :: analogReference(INTERNAL);
      :volt = analogRead(VBAT_ADC) * 1.1 / 1023.00;
      :bat_voltage = (volt * (VD_R1 + VD_R2) / (VD_R2));
      :Print "Battery Voltage 3 :: `bat_voltage`";
      if (bat_voltage <= BAT_LOW_ALERT) then (yes);
        :Print "*******************LOW_BAT*******************";
        :Turn GREEN_LED ON;
      else (no);
        :Turn GREEN_LED OFF;
      endif
    endif
    end
    @enduml */
void check_vbat()
{
  if ((millis() - printtime > 1000))// checking the current time with the timer and also to make sure that this loop wont execute unlimitedly.
  {
    Serial.println(F("Check Bat Voltage"));
    printtime = millis(); /// ensure that we initialise timer with current value, so that the condition in if statement doesnt satisfy repeatedly.
    //
    analogReference(INTERNAL);
    volt = analogRead(VBAT_ADC) * 1.1 / 1023.00; //(10K-VD_R1,3.9K-VD_R2)
    bat_voltage = (volt * (VD_R1 + VD_R2) / (VD_R2));
    Serial.println(F("\n\n\n\n\n"));
    Serial.print(F("\nBattery Voltage 3 :: ")); Serial.println(bat_voltage);

    Serial.println(F("\n\n\n\n\n"));
    if(bat_voltage >= LED3_TH_V)
    {
      digitalWrite(LED_IND3, HIGH);
      analogWrite(LED_IND2, 255);
      analogWrite(LED_IND1, 255);
      digitalWrite(GREEN_LED, HIGH);
      BatteryVolts_IND = 3;

      #ifdef DEBUG_FW412
        Serial.println(F("LED_IND3 HIGH .."));
      #endif
    
    }
    else if(bat_voltage >= LED2_TH_V)
    {
      digitalWrite(LED_IND3, HIGH);
      analogWrite(LED_IND2, 255);
      analogWrite(LED_IND1, 0);
      digitalWrite(GREEN_LED, HIGH);
      BatteryVolts_IND = 2;
      
      #ifdef DEBUG_FW412
        Serial.println(F("LED_IND2 HIGH .."));
      #endif
    
    }
    else if(bat_voltage >= LED1_TH_V)
    {
      digitalWrite(LED_IND3, HIGH);
      analogWrite(LED_IND2, 0);
      analogWrite(LED_IND1, 0);
      digitalWrite(GREEN_LED, HIGH);
      BatteryVolts_IND = 1;
      
      #ifdef DEBUG_FW412
        Serial.println(F("LED_IND1 HIGH .."));
      #endif
    
    }
    else if(bat_voltage < BAT_LOW_ALERT) 
    {
      Serial.println(F("*******************LOW_BAT*******************"));
      digitalWrite(LED_IND3, LOW);
      analogWrite(LED_IND2, 0);
      analogWrite(LED_IND1, 0);
      digitalWrite(GREEN_LED, HIGH);
      BatteryVolts_IND = 0;

      #ifdef DEBUG_FW412
        Serial.println(F("GREEN_LED HIGH .."));
      #endif
      
    }
    #ifdef DEBUG_FW412
      Serial.print(F("BatteryVolts_IND value: "));
      Serial.println(BatteryVolts_IND);
      delay(5); //small delay for Serial print
    #endif
  }
}

/** @def PRINT_DW_ID
    @brief This MACRO if enabled prints the Device ID, Unique ID, Network ID, Device Address and Device Mode for Decawave */
#define PRINT_DW_ID
/** @brief Initializes Decawave 1000 aka DW1000 in code
    @startuml{decawaveInit.png} "Activity of decawaveInit" width=5cm
    start
    :Begin Decawave and set Interrupt and Reset Pin :: DW1000.begin(PIN_IRQ, PIN_RST);
    :Set SPI Select PIN as PIN_SS for DW1000;
    :Reset the DW1000;
    :Print "DW1000 initializing ...";
    :DW1000.newConfiguration();
    :DW1000.setDefaults();
    :DW1000.setDeviceAddress(1);
    :DW1000.setNetworkId(10);
    :DW1000.enableMode(DW1000.MODE_SHORTDATA_FAST_ACCURACY);
    :DW1000.commitConfiguration();
    end
    @enduml */
void decawaveInit() {

  DW1000.begin(PIN_IRQ, PIN_RST);
  DW1000.select(PIN_SS);
  DW1000.reset();

  Serial.println(F("DW1000 initializing ..."));
  DW1000.newConfiguration();
  DW1000.setDefaults();
  DW1000.setDeviceAddress(1);
  DW1000.setNetworkId(10);
  DW1000.enableMode(DW1000.MODE_SHORTDATA_FAST_ACCURACY);
  DW1000.commitConfiguration();

#ifdef PRINT_DW_ID
  Serial.println(F("Committed configuration ..."));
  char msg[128];
  DW1000.getPrintableDeviceIdentifier(msg);
  Serial.print(F("Device ID: ")); Serial.println(msg);
  DW1000.getPrintableExtendedUniqueIdentifier(msg);
  Serial.print(F("Unique ID: ")); Serial.println(msg);
  DW1000.getPrintableNetworkIdAndShortAddress(msg);
  Serial.print(F("Network ID & Device Address: ")); Serial.println(msg);
  DW1000.getPrintableDeviceMode(msg);
  Serial.print(F("Device mode: ")); Serial.println(msg);
#endif
}

/** @brief This function sets up Device Settings related to Safmet
    @startuml{setupDeviceSettings.png} "Activity Diagram of setupDeviceSettings" width=5cm
    start
    :Set 4 byte device_ID;
    :Set Sub_Net_ID;
    :Set Net_ID;
    :Set param.beacon_timeout;
    :Set Ignore_UIDs as 0xFFFFFFFF
    :Set sleep_timeout, temp_near, temp_emerg, dist_n_f, dist_e_f, dist_n_f_f, dist_e_f_f as 0xFF;
    if (def DEBUG_SETTINGS) then (yes);
      :Print sleep_timeout, temp_near, temp_emerg, dist_n_f, dist_e_f, dist_n_f_f, dist_e_f_f;
    endif
    stop
    @enduml */
void setupDeviceSettings() {
  //Device Address
  device_ID[0] = dev_ID[0];//csk
  device_ID[1] = dev_ID[1];//csk
  device_ID[2] = dev_ID[2];//csk
  device_ID[3] = dev_ID[3];//csk

  device.Sub_Net_ID = 0XABAB;//csk
  device.Net_ID = 0XFF02;//csk
  param.beacon_timeout = 300;// timeout is 300ms //csk

  param.en_func = 0x0; //csk
  param.tx_pwr = 0xFF;//csk
  param.rx_pwr = 0xFF;//csk
  param.op_mode = BEACON;//csk
  param.sleep_time = 0x05;//csk
  param.wait_time = 0x64;//csk

  //*CSK
  for (int i = 0; i < 4; i++)
  {
    I_UD.Ignore_UID[i] = 0xFFFFFFFF;
  }

  dist_para.dist_SID[0] = 0xFFFFFFFF;  //what is this parameters and where it is use??????
  dist_para.dist_SID[1] = 0xFFFFFFFF;
  dist_para.dist_SID[2] = 0xFFFFFFFF;
  dist_para.dist_SID[3] = 0xFFFFFFFF;
  dist_para.dist_SID[4] = 0xAFAF0606;
  
  sleep_timeout = 0xFF;
  temp_near = 0xFF;
  temp_emerg = 0xFF;
  dist_n_f = 0xFF;
  dist_e_f = 0xFF;
  dist_n_f_f = 0xFF;
  dist_e_f_f = 0xFF;

#ifdef DEBUG_SETTINGS
  Serial.print(F("sleep_timeout:")); Serial.println(sleep_timeout);//csk
  Serial.print(F("temp_near:")); Serial.println(temp_near);//csk
  Serial.print(F("temp_emerg:")); Serial.println(temp_emerg);//csk
  Serial.print(F("dist_n_f:")); Serial.println(dist_n_f);//csk
  Serial.print(F("dist_e_f:")); Serial.println(dist_e_f);//csk/
  Serial.print(F("dist_n_f_f:")); Serial.println(dist_n_f_f);//csk
  Serial.print(F("dist_e_f_f:")); Serial.println(dist_e_f_f);//csk
#endif
}

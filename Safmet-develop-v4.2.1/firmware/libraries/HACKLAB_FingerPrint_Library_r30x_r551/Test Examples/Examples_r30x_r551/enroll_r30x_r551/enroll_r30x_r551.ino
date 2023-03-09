/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <HACKLAB_FingerPrint_Library_r30x_r551.h>
#include <ArduinoOTA.h>
#include <WiFi.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial

HardwareSerial FingerPrint(1);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FingerPrint);



void updateCheck()
{
  WiFi.begin("HACKLAB", "UPDATE12");
  bool connect_flag = false;
  uint32_t prev_millis = 0;
  while ((WiFi.status() != WL_CONNECTED) && (millis() - prev_millis < 4000)) {
    Serial.print(".");
    //prev_millis = millis();
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    //timerWrite(timer, 0);
    connect_flag = true;
    Serial.print("Connected. Upload at ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("timer_expired");
  }
  while (connect_flag)
  {
    //timerWrite(timer, 0);
    ArduinoOTA.begin();
    ArduinoOTA.handle();
  }
}


uint8_t id;

void setup()
{
  Serial.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");

  //updateCheck();
  FingerPrint.begin(57600, SERIAL_8N1, 27, 26);
  //FingerPrint.begin(57600, SERIAL_8N1, 16, 17);
  // set the data rate for the sensor serial port
  //finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  //finger.emptyDatabase();
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{
  Serial.println("Ready to enroll a fingerprint!");
  Serial.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  id = readnumber();
  if (id == 0) {// ID #0 not allowed, try again!
    return;
  }
  Serial.print("Enrolling ID #");
  Serial.println(id);

  while (!  getFingerprintEnroll() );
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.print(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(id);

  //getModel();

  Serial.println("model printed");

  p = finger.storeModel(id);
  while (p != FINGERPRINT_OK) {
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
      Serial.println("Stored!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");

    } else if (p == FINGERPRINT_BADLOCATION) {
      Serial.println("Could not store in that location");
    } else if (p == FINGERPRINT_FLASHERR) {
      Serial.println("Error writing to flash");
    } else {
      Serial.println(p);
      Serial.println("Unknown error");

    }
    delay(10);
  }
  //getModel();
  return 1;
}

int getModel() {
  uint8_t bytesReceived[496];

  uint8_t  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print(F("Template ")); Serial.print(5); Serial.println(F(" transferring:"));
      break;
    default:
      Serial.print(F("Unknown error ")); Serial.println(p);
      return p;
  }

  // one data packet is 267 bytes. in one data packet, 11 bytes are 'usesless' :D
  // 2 data packets
  memset(bytesReceived, 0xff, 496);

  uint32_t starttime = millis();
  int i = 0;
  while (i < 496 && (millis() - starttime) < 20000) {
    if (FingerPrint.available()) {
      bytesReceived[i++] = FingerPrint.read();
    }
  }

  for (int i = 0; i < 496; ++i) {
    Serial.print(F("0x"));
    printHex(bytesReceived[i], 2);
    Serial.print(F(", "));
  }
  Serial.println();

}

void printHex(int num, int precision) {
  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial.print(tmp);
}

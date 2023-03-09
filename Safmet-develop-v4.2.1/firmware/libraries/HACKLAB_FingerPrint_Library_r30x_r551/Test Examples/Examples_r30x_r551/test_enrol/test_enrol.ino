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
#define FP_P
// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial

HardwareSerial FingerPrint(1);
HardwareSerial Software(0);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FingerPrint);

void updateCheck()
{
  WiFi.begin("HACKLAB", "UPDATE12");
  bool connect_flag = false;
  uint32_t prev_millis = 0;
  while ((WiFi.status() != WL_CONNECTED) && (millis() - prev_millis < 4000)) {
    Software.print(".");
    //prev_millis = millis();
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    //timerWrite(timer, 0);
    connect_flag = true;
    Software.print("Connected. Upload at ");
    Software.println(WiFi.localIP());
  }
  else
  {
    Software.println("timer_expired");
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
  Software.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Software.println("\n\nAdafruit Fingerprint sensor enrollment");

  //updateCheck();
  FingerPrint.begin(57600, SERIAL_8N1, 27, 26);
  //FingerPrint.begin(57600, SERIAL_8N1, 16, 17);
  // set the data rate for the sensor serial port
  //finger.begin(57600);

  if (finger.verifyPassword()) {
    Software.println("Found fingerprint sensor!");
  } else {
    Software.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  finger.getTemplateCount();
  Software.print(F("Sensor contains ")); Software.print(finger.templateCount); Software.println(F(" templates"));
  Software.println("Ready to enroll a fingerprint!");
  Software.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");

  //finger.emptyDatabase();
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Software.available());
    num = Software.parseInt();
  }
  return num;
}

void loop()                     // run over and over again
{
  if (Software.available() > 0)
  {
    String inString = Software.readString();
    if (inString == "del")
    {
      finger.emptyDatabase();
      Software.println("Now database is empty :)");
      finger.getTemplateCount();
      Software.print(F("Sensor contains ")); Software.print(finger.templateCount); Software.println(F(" templates"));
      Software.println("Ready to enroll a fingerprint!");
      Software.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");

      return;
    }

    id = inString.toInt();
    if (id == 0)
    {
      return;
    }
    Software.print("Enrolling ID #");
    Software.println(id);
    while (!  getFingerprintEnroll() );
    finger.getTemplateCount();
    Software.print(F("Sensor contains ")); Software.print(finger.templateCount); Software.println(F(" templates"));
    Software.println("Ready to enroll a fingerprint!");
    Software.println("Please type in the ID # (from 1 to 127) you want to save this finger as...");
  }
}


uint8_t getFingerprintEnroll() {
  //static uint32_t enr_tim = 0;
  int p = -1;
#ifdef FP_P
  Software.print("Waiting for valid finger to enroll as #"); Software.println(id);
#endif
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
#ifdef FP_P
        Software.println("Image taken");
#endif
        break;
      case FINGERPRINT_NOFINGER:
#ifdef FP_P
        Software.print(".");
#endif
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
#ifdef FP_P
        Software.println("Communication error");
#endif
        break;
      case FINGERPRINT_IMAGEFAIL:
#ifdef FP_P
        Software.println("Imaging error");
#endif
        break;
      default:
#ifdef FP_P
        Software.println("Unknown error");
#endif
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
#ifdef FP_P
      Software.println("Image converted");
#endif
      break;
    case FINGERPRINT_IMAGEMESS:
#ifdef FP_P
      Software.println("Image too messy");
#endif
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
#ifdef FP_P
      Software.println("Communication error");
#endif
      return p;
    case FINGERPRINT_FEATUREFAIL:
#ifdef FP_P
      Software.println("Could not find fingerprint features");
#endif
      return p;
    case FINGERPRINT_INVALIDIMAGE:
#ifdef FP_P
      Software.println("Could not find fingerprint features");
#endif
      return p;
    default:
#ifdef FP_P
      Software.println("Unknown error");
#endif
      return p;
  }
#ifdef FP_P
  Software.println("Remove finger");
#endif
  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
#ifdef FP_P
  Software.print("ID "); Software.println(id);
#endif
  p = -1;
#ifdef FP_P
  Software.println("Place same finger again");
#endif
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
#ifdef FP_P
        Software.println("Image taken");
#endif
        break;
      case FINGERPRINT_NOFINGER:
#ifdef FP_P
        Software.print(".");
#endif
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
#ifdef FP_P
        Software.println("Communication error");
#endif
        break;
      case FINGERPRINT_IMAGEFAIL:
#ifdef FP_P
        Software.println("Imaging error");
#endif
        break;
      default:
#ifdef FP_P
        Software.println("Unknown error");
#endif
        break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
#ifdef FP_P
      Software.println("Image converted");
#endif
      break;
    case FINGERPRINT_IMAGEMESS:
#ifdef FP_P
      Software.println("Image too messy");
#endif
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
#ifdef FP_P
      Software.println("Communication error");
#endif
      return p;
    case FINGERPRINT_FEATUREFAIL:
#ifdef FP_P
      Software.println("Could not find fingerprint features");
#endif
      return p;
    case FINGERPRINT_INVALIDIMAGE:
#ifdef FP_P
      Software.println("Could not find fingerprint features");
#endif
      return p;
    default:
#ifdef FP_P
      Software.println("Unknown error");
#endif
      return p;
  }

  // OK converted!
#ifdef FP_P
  Software.print("Creating model for #");  Software.println(id);
#endif

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
#ifdef FP_P
    Software.println("Prints matched!");
#endif
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
#ifdef FP_P
    Software.println("Communication error");
#endif
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
#ifdef FP_P
    Software.println("Fingerprints did not match");
#endif
    return p;
  } else {
#ifdef FP_P
    Software.println("Unknown error");
#endif
    return p;
  }


  p = finger.storeModel(id);
  while (p != FINGERPRINT_OK) {
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
#ifdef FP_P
      Software.println("Stored!");
#endif
      //    return p;
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
#ifdef FP_P
      Software.println("Communication error");
#endif
      //    return p;
    } else if (p == FINGERPRINT_BADLOCATION) {
#ifdef FP_P
      Software.println("Could not store in that location");
#endif
      //    return p;
    } else if (p == FINGERPRINT_FLASHERR) {
#ifdef FP_P
      Software.println("Error writing to flash");
#endif
      //    return p;
    } else {
#ifdef FP_P
      Software.println(p);
      Software.println("Unknown error");
#endif
      //return p;
    }
    delay(10);
  }
  //getModel();
#ifdef FP_P
   Software.println("Stored!!!!");
#endif
  return 1;
}

int getModel() {
  uint8_t bytesReceived[496];

  uint8_t  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      Software.print(F("Template ")); Software.print(5); Software.println(F(" transferring:"));
      break;
    default:
      Software.print(F("Unknown error ")); Software.println(p);
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
    Software.print(F("0x"));
    printHex(bytesReceived[i], 2);
    Software.print(F(", "));
  }
  Software.println();

}

void printHex(int num, int precision) {
  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Software.print(tmp);
}

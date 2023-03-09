//#define r307
//#define r551
#define r502

#include "SPIFFS.h"
#include "FS.h"
#include <ArduinoJson.h>

#include <HACKLAB_FingerPrint_Library_r30x_r551.h>
HardwareSerial FingerPrint(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FingerPrint);
String temple;

#ifdef  r307
const int bytes = 534;
#endif

#ifdef r502
#define bytes 768
#endif

#ifdef r551
const int bytes = 496;
#endif

String streeng;
uint8_t bytesReceived[bytes]; // 2 data packets
//uint8_t bytesReceived2[bytes];
uint8_t getFingerprintEnroll();
int getModel();
uint8_t getFingerprintEnroll();
uint8_t deleteFingerprint(uint8_t id);
int id;
void printHex(int num, int precision);
uint8_t downloadFingerprintTemplate(uint16_t id);
uint32_t lastdot;
int count;
String received;

void resetFP()
{
  delay(100);
  FingerPrint.end();
  delay(100);
  FingerPrint.begin(57600, SERIAL_8N1, 27, 26);
  delay(300);

  if (finger.verifyPassword())
  {
    //    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
}




void setup()
{
  while (!Serial);
  Serial.begin(115200);
  Serial.println("\n\n\n\n\n\n********************************************\nFingerprint All In One Test Code");
  resetFP();
  finger.getTemplateCount();
  count = String(finger.templateCount).toInt();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  SPIFFS.begin();
}

void loop()
{
  //Serial.print("0");
  if (Serial.available() > 0)
  {
    received = Serial.readString();
    Serial.print("received "); Serial.println(received);
    int n = received.length();
    if (received.startsWith("enrol")) //done
    {
      String Data = received.substring(5, n);
      id = Data.toInt();

      Serial.print("Enrolling ID #");
      Serial.println(id);

      while (!getFingerprintEnroll() );
    }
    else if (received.startsWith("delete"))
    {
      String Data = received.substring(6, n);
      id = Data.toInt();
      Serial.print("Deleting ID #");
      Serial.println(id);
      resetFP();
      deleteFingerprint(id);
      resetFP();
    }
    else if (received.startsWith("empty"))
    {
      resetFP();
      finger.emptyDatabase();
      resetFP();
      Serial.println("Now database is empty :)");
    }
    else if (received.startsWith("showAll"))
    {
      for (int i = 0; i < 10; i++)
      {
        resetFP();
        downloadFingerprintTemplate(i);
        resetFP();
      }
      Serial.println("\nAfter Showing Templates");
    }
    else if (received.startsWith("show"))
    {
      //Serial.print("1,");
      String DATA = received.substring(4, n);
      id = DATA.toInt();
      Serial.print("Showing Templates");
      Serial.println(id);
      resetFP();
      downloadFingerprintTemplate(id);
      resetFP();
      Serial.println("\nAfter Showing Templates");
    }
    else if (received.startsWith("move"))
    {
      String DATA = received.substring(4, n);
      int r = DATA.length();
      int startp = 0;
      int endp = 1;
      int comma1p = DATA.indexOf(',', startp);
      endp = DATA.indexOf(',', comma1p + 1);

      String oldID = DATA.substring(startp, comma1p);
      int oldIDee = oldID.toInt();
      Serial.print("Old ID #");
      Serial.println(oldIDee);
      //#ifdef r307
      resetFP();
      downloadFingerprintTemplate(oldIDee);
      resetFP();
      //#endif

      //#ifdef r551
      //      uint8_t hello[bytes] = {239, 1, 255, 255, 255, 255, 2, 0, 130, 4, 12, 96, 0, 114, 106, 136, 126, 196, 72, 56, 8, 18, 99, 134, 106, 196, 73, 118, 8, 90, 219, 132, 89, 68, 89, 254, 15, 93, 51, 135, 2, 200, 136, 137, 238, 218, 155, 136, 135, 198, 106, 112, 224, 18, 140, 135, 114, 76, 138, 48, 248, 49, 188, 133, 82, 134, 87, 62, 40, 74, 172, 136, 37, 7, 109, 255, 239, 90, 52, 135, 232, 77, 108, 211, 239, 90, 36, 136, 127, 79, 141, 172, 248, 210, 25, 134, 117, 65, 43, 246, 239, 17, 186, 134, 239, 130, 186, 5, 255, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 47, 135, 239, 1, 255, 255, 255, 255, 2, 0, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 132, 239, 1, 255, 255, 255, 255, 2, 0, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 147, 71, 113, 99, 71, 55, 69, 42, 35, 195, 17, 65, 2, 73, 38, 136, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 80, 239, 1, 255, 255, 255, 255, 2, 0, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 96, 219, 108, 84, 49};
      //      for (int rr = 0; rr < bytes; rr++)
      //      {
      //        bytesReceived[rr] = hello[rr];
      //      }
      //#endif
      //
      //#ifdef r502
      //      uint8_t hello[bytes] = {239, 1, 255, 255, 255, 255, 2, 0, 130, 2, 1, 104, 69, 139, 162, 99, 0, 132, 220, 12, 217, 112, 246, 9, 6, 181, 133, 50, 132, 169, 28, 225, 73, 129, 124, 35, 172, 82, 68, 204, 148, 199, 20, 103, 57, 178, 243, 133, 32, 144, 131, 10, 97, 72, 132, 44, 203, 236, 82, 209, 54, 240, 207, 20, 173, 80, 79, 12, 197, 63, 213, 82, 111, 145, 79, 245, 131, 142, 252, 82, 233, 98, 132, 160, 20, 167, 142, 139, 152, 107, 0, 132, 199, 12, 201, 2, 231, 7, 11, 52, 197, 48, 67, 169, 124, 225, 73, 97, 68, 51, 180, 82, 64, 190, 150, 195, 20, 100, 53, 180, 243, 133, 31, 15, 179, 111, 177, 74, 195, 240, 226, 144, 82, 29, 38, 244, 205, 60, 83, 239, 1, 255, 255, 255, 255, 2, 0, 130, 20, 165, 75, 207, 141, 69, 63, 212, 50, 175, 17, 79, 245, 75, 150, 244, 82, 205, 84, 138, 158, 20, 162, 142, 139, 142, 107, 0, 132, 185, 12, 189, 189, 250, 8, 9, 179, 133, 49, 132, 41, 156, 161, 73, 97, 84, 67, 164, 82, 76, 204, 150, 195, 20, 102, 56, 53, 115, 133, 32, 80, 51, 239, 145, 75, 52, 16, 242, 144, 82, 37, 46, 250, 205, 20, 171, 77, 208, 140, 69, 63, 212, 178, 207, 17, 79, 245, 107, 174, 244, 82, 225, 94, 140, 150, 20, 166, 142, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 144, 239, 1, 255, 255, 255, 255, 2, 0, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 132, 239, 1, 255, 255, 255, 255, 2, 0, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 132, 239, 1, 255, 255, 255, 255, 2, 0, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 132, 239, 1, 255, 255, 255, 255, 2, 0, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
      //      for (int rr = 0; rr < bytes; rr++)
      //      {
      //        bytesReceived[rr] = hello[rr];
      //      }
      //#endif


      String newID = DATA.substring(comma1p + 1, endp);
      int newIDee = newID.toInt();
      Serial.print("New ID #");
      Serial.println(newIDee);

      uint8_t  p = 0;
      Serial.println(F("\nTrying to download the model now"));
      do
      {
        delay(10);
        resetFP();
        p = finger.downloadModel(0x01);
        resetFP();
        if (p == FINGERPRINT_OK)
        {
          Serial.println("Ready to Download Model");
        }
        if (p == FINGERPRINT_PACKETRECIEVEERR)
        {
          Serial.println("FINGERPRINT_PACKETRECIEVEERR");
        }
      }
      while (p);
      //      finger.writeFingerPrintPacket(bytesReceived);
      delay(1000);

      do
      {
        delay(10);
        resetFP();
        p = finger.storeModel(newIDee);
        resetFP();
      } while (p);

      if (p == FINGERPRINT_OK) {
        Serial.println(F("Prints matched!"));
      } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
        Serial.println(F("Communication error"));
        //return ;
      } else if (p == FINGERPRINT_ENROLLMISMATCH) {
        Serial.println(F("Fingerprints did not match"));
        //return ;
      } else {
        Serial.println(F("Unknown error"));
        //return ;
      }
      delay(2000);
      resetFP();
      p = finger.getTemplateCount();
      resetFP();
      Serial.println(p);
      Serial.print(F("Sensor contains "));  Serial.print(finger.templateCount);  Serial.println(F(" templates after adding"));
      resetFP();
      deleteFingerprint(oldIDee);
      resetFP();
      p = finger.getTemplateCount();
      resetFP();
      Serial.println(p);
      Serial.print(F("Sensor contains "));  Serial.print(finger.templateCount);  Serial.println(F(" templates after deleting"));
    }
    else
    {
      Serial.println("Unknown Command");
    }
    //Serial.print("2,");
  }
  else
  {
    //Serial.println("\n no serial");
    resetFP();
    getFingerprintIDez();
    resetFP();
    //Serial.println("\n no serial2");
    if (millis() - lastdot > 1000)
    {
      Serial.print(".");
      lastdot = millis();
    }
  }
  //Serial.print("9,");
  if (received.startsWith("show"))
  {
    Serial.println("Hello Command");
    received = "";
    delay(1000);
    //      for (int g = 0; g < 50; g++)
    //      {
    //        Serial.print("f");
    //        delay(1);
    //      }
    //    while (1);
  }
  //Serial.print("10,");
}

uint8_t getFingerprintEnroll()
{
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    resetFP();
    p = finger.getImage();
    resetFP();
    switch (p)
    {
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
  resetFP();
  p = finger.image2Tz(1);
  resetFP();
  switch (p)
  {
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
  while (p != FINGERPRINT_NOFINGER)
  {
    resetFP();
    p = finger.getImage();
    resetFP();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK)
  {
    resetFP();
    p = finger.getImage();
    resetFP();
    switch (p)
    {
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
  resetFP();
  p = finger.image2Tz(2);
  resetFP();
  switch (p)
  {
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
  resetFP();
  p = finger.createModel();
  resetFP();
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
  resetFP();
  p = finger.storeModel(id);
  resetFP();
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
/*
  int getModel() {
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
  memset(bytesReceived, 0xff, 496);//bytes

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
*/

uint8_t deleteFingerprint(uint8_t id)
{
  uint8_t p = -1;
  resetFP();
  p = finger.deleteModel(id);
  resetFP();
  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }
}


//uint8_t Adafruit_Fingerprint::downloadModel(uint16_t id, uint8_t packet_1[], uint8_t packet_2[], uint8_t packet_3[], uint8_t packet_4[], uint8_t slot)
//{
//  uint8_t packet[] = {FINGERPRINT_DOWNLOAD, slot};
//  writePacket(theAddress, FINGERPRINT_COMMANDPACKET, sizeof(packet) + 2, packet);
//  uint8_t len = getReply(recvPacket);
//  uint8_t x;
//  if ((len != 1) && (recvPacket[0] != FINGERPRINT_ACKPACKET))
//    return -1;
//  if (recvPacket[1] == 0x00)
//  {
//    writePacket(theAddress, 0x02, 66, packet_1);
//    writePacket(theAddress, 0x02, 66, packet_2);
//    writePacket(theAddress, 0x02, 66, packet_3);
//    writePacket(theAddress, 0x08, 66, packet_4);
//    return recvPacket[1];
//  }
//  else {
//    return -1;
//  }
//}



void printHex(int num, int precision)
{

  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial.print(tmp);
  temple += String(tmp);
}


uint8_t downloadFingerprintTemplate(uint16_t id)
{
  Serial.println("------------------------------------");
  Serial.print("Attempting to load #"); Serial.println(id);
  uint8_t p = finger.loadModel(id);
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.print("Template "); Serial.print(id); Serial.println(" loaded");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  // OK success!

  Serial.print("Attempting to get #"); Serial.println(id);
  p = finger.getModel();
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.print("Template "); Serial.print(id); Serial.println(" transferring:");
      break;
    default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  uint32_t starttime = millis();
  int i = 0;
  while (i < 534 && (millis() - starttime) < 20000)
  {
    if (FingerPrint.available() > 0)
    {
      bytesReceived[i++] = FingerPrint.read();
    }
  }
  Serial.print(i); Serial.println(" bytes read.");
  Serial.println("Decoding packet...");
  temple = "";
  for (int i = 0; i < 534; ++i)
  {
    Serial.print("0x");
    //temple += "0x";
    printHex(bytesReceived[i], 2);
    Serial.print(", ");
    temple += ",";
  }
  //  Serial.println("\n\n");
  //  Serial.println(temple);
  Serial.println("\ndone.");
  resetFP();
}






/*
  uint8_t downloadFingerprintTemplate(uint16_t id)
  {
  Serial.println("------------------------------------");
  //  if (received.startsWith("show"))
  {
    Serial.print("Attempting to load #"); Serial.println(id);
  }
  uint8_t p = finger.loadModel(id);
  switch (p)
  {
    case FINGERPRINT_OK:
      //      if (received.startsWith("show"))
      {
        Serial.print("Template "); Serial.print(id); Serial.println(" loaded");
      }
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  // OK success!
  if (received.startsWith("show"))
  {
    Serial.print("Attempting to get #"); Serial.println(id);
  }
  p = finger.getModel();
  switch (p)
  {
    case FINGERPRINT_OK:
      Serial.print("Template "); Serial.print(id); Serial.println(" transferring:");
      break;
    default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  if (received.startsWith("move"))  Serial.println(F("Appending"));
  uint32_t starttime = millis();
  int i = 0;
  String appendLog;
  while (i < 534)// && ((millis() - starttime) < 20000))
  {
    while (!(FingerPrint.available() > 0) && ((millis() - starttime) < 20000))  yield;
    uint8_t num = FingerPrint.read();
    appendLog = String(num);
  //    appendLog += ",";
    if (received.startsWith("show"))  Serial.print(appendLog);
    i++;
  }

  if (received.startsWith("move"))
  {
    String nameO = "/id";
    nameO += id;
    nameO += ".txt";
    writeFile(SPIFFS, "/fpRI.txt", appendLog);
  }

  FingerPrint.end();
  delay(50);
  FingerPrint.begin(57600, SERIAL_8N1, 27, 26);
  delay(100);
  if (finger.verifyPassword())
  {
    if (received.startsWith("move"))    Serial.println(F("\nAfter Moving"));
    if (received.startsWith("show"))    Serial.println(F("\nAfter Showing"));
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  }
*/
int getFingerprintIDez()
{
  resetFP();
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  {

    return -1;
  }
  Serial.println();
  Serial.println("got image");
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  //Serial.println("got characteristics");

#ifdef r307
  p = finger.fingerFastSearch();
#else
  p = finger.fingerSearch();
#endif

  if (p != FINGERPRINT_OK) {
    Serial.println("Not a match!!!");
    return -1;
  }
  //Serial.println("search done found");

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

void deleteFile(fs::FS &fs, const char * path)
{
  resetFP();
  //  Serial.println("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println(F("- file deleted"));
  } else {
    Serial.println(F("- delete failed"));
  }
}

void writeFile(fs::FS & fs, const char * path, String message)
{
  resetFP();
#ifdef ENROL_P
  //  Serial.printf("Writing file: %s\r\n", path);
#endif
  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println(F("- failed to open file for writing"));
  }
  if (file.print(message))
  {
    //#ifdef ENROL_P
    //    Serial.println(F("- file written"));
    //#endif
  }
  else
  {
    Serial.println(F("- frite failed"));
  }
}

void readFile(fs::FS & fs, const char * path)
{
  resetFP();
  Serial.printf("Opened to read the file: %s\r\n", path);

  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println(F("- failed to open file for reading"));
    return;
  }

  Serial.print(F("Reading :"));
  String abc;
  if (file.available())
  {
    abc = file.readString();
  }
  streeng = abc;
  Serial.println(F("Complete"));
}

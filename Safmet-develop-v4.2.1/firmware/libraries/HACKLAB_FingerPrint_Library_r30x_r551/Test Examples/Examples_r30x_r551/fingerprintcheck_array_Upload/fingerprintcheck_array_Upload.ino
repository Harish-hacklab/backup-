#define R307

#include <HACKLAB_FingerPrint_Library_r30x_r551.h>
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#ifdef R307
byte fingerp[] = {139, 14, 187, 21, 139, 29, 147, 28, 234, 33, 156, 153, 6, 94, 171, 147, 38, 33, 42, 88, 2, 240, 0, 192, 0, 192, 0, 128, 0, 128, 0, 128, 0, 192, 0, 192, 0, 192, 0, 192, 0, 224, 0, 240, 2, 248, 6, 255, 254, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 119, 25, 225, 222, 85, 30, 80, 254, 64, 49, 214, 190, 40, 178, 25, 222, 102, 50, 18, 94, 86, 60, 233, 254, 80, 25, 227, 31, 67, 30, 214, 156, 29, 41, 95, 124, 69, 141, 11, 53, 76, 142, 74, 93, 75, 44, 41, 186, 23, 157, 72, 118, 74, 168, 104, 86, 70, 168, 21, 183, 30, 25, 222, 20, 61, 17, 98, 18, 33, 153, 200, 83, 46, 87, 239, 1, 255, 255, 255, 255, 2, 0, 130, 67, 24, 99, 13, 37, 161, 134, 77, 29, 33, 8, 170, 38, 166, 200, 102, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 210, 239, 1, 255, 255, 255, 255, 2, 0, 130, 3, 1, 80, 50, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 65, 162, 105, 158, 58, 41, 150, 94, 108, 180, 147, 94, 77, 180, 234, 94, 62, 190, 212, 191, 78, 194, 84, 223, 110, 59, 233, 95, 37, 61, 151, 127, 70, 56, 212, 190, 111, 191, 19, 127, 123, 161, 229, 62, 79, 17, 227, 223, 77, 132, 76, 250, 81, 134, 11, 219, 44, 5, 225, 54, 29, 143, 223, 20, 5, 61, 133, 51, 25, 143, 200, 49, 34, 230, 239, 1, 255, 255, 255, 255, 8, 0, 130, 34, 42, 154, 206, 94, 171, 147, 174, 47, 194, 43, 111, 22, 191, 84, 142, 52, 68, 20, 174, 84, 23, 81, 79, 91, 190, 83, 15, 117, 148, 99, 206, 56, 138, 10, 174, 54, 11, 33, 142, 73, 59, 170, 236, 28, 64, 193, 13, 13, 175, 130, 141, 34, 192, 64, 141, 87, 194, 169, 76, 66, 62, 234, 141, 67, 146, 201, 140, 114, 195, 82, 75, 62, 152, 168, 42, 10, 61, 95, 10, 7, 185, 71, 203, 12, 188, 3, 139, 14, 187, 21, 139, 29, 147, 28, 234, 33, 156, 153, 6, 94, 171, 147, 38, 33, 42, 88, 6, };
#else
byte fingerp[] = {239, 1, 255, 255, 255, 255, 2, 0, 130, 2, 4, 252, 187, 139, 115, 187, 0, 132, 196, 18, 192, 76, 233, 9, 200, 199, 197, 37, 130, 232, 48, 17, 72, 81, 146, 120, 144, 81, 180, 144, 22, 216, 20, 105, 45, 161, 64, 197, 19, 76, 13, 82, 241, 69, 51, 49, 120, 84, 81, 20, 229, 92, 107, 20, 121, 59, 45, 153, 69, 18, 206, 233, 210, 113, 69, 20, 202, 133, 192, 81, 5, 52, 119, 75, 20, 94, 78, 24, 180, 5, 27, 85, 6, 249, 145, 72, 37, 84, 130, 52, 81, 185, 86, 75, 74, 20, 107, 87, 36, 233, 69, 21, 22, 18, 53, 193, 71, 63, 142, 139, 40, 219, 0, 132, 155, 13, 148, 165, 220, 8, 89, 18, 133, 48, 131, 13, 35, 50, 90, 239, 1, 255, 255, 255, 255, 2, 0, 130, 33, 73, 97, 109, 68, 176, 82, 152, 134, 113, 9, 20, 139, 38, 67, 202, 197, 30, 76, 39, 227, 65, 71, 132, 65, 100, 68, 82, 101, 17, 100, 92, 20, 139, 71, 140, 9, 197, 43, 82, 170, 115, 209, 72, 85, 41, 164, 180, 83, 9, 82, 130, 117, 20, 189, 86, 203, 27, 197, 38, 255, 142, 139, 83, 83, 0, 132, 163, 13, 166, 76, 173, 7, 169, 62, 5, 42, 67, 117, 17, 1, 74, 176, 251, 48, 72, 82, 84, 135, 47, 13, 20, 134, 45, 37, 185, 197, 25, 75, 194, 108, 161, 72, 115, 146, 248, 72, 81, 141, 53, 11, 31, 20, 131, 77, 148, 177, 69, 36, 83, 203, 175, 49, 72, 229, 114, 127, 124, 50, 103, 239, 1, 255, 255, 255, 255, 2, 0, 130, 82, 121, 102, 250, 73, 20, 169, 91, 184, 70, 69, 42, 191, 142, 139, 52, 227, 0, 132, 154, 12, 153, 155, 107, 7, 185, 136, 5, 43, 199, 98, 173, 97, 72, 113, 228, 172, 104, 82, 164, 134, 150, 251, 20, 110, 47, 173, 136, 5, 25, 78, 227, 44, 225, 72, 148, 74, 124, 20, 83, 253, 22, 239, 43, 20, 126, 74, 31, 133, 69, 63, 211, 97, 11, 161, 74, 245, 34, 236, 148, 83, 253, 72, 214, 101, 20, 255, 142, 139, 86, 187, 0, 132, 0, 17, 0, 72, 143, 14, 167, 58, 69, 32, 196, 66, 236, 161, 73, 33, 155, 40, 72, 81, 168, 179, 19, 31, 20, 86, 45, 20, 47, 133, 23, 139, 204, 47, 49, 53, 194, 239, 1, 255, 255, 255, 255, 2, 0, 130, 68, 163, 114, 159, 108, 80, 244, 229, 4, 87, 20, 92, 59, 58, 69, 197, 20, 16, 35, 233, 225, 70, 4, 179, 245, 112, 81, 65, 60, 213, 63, 20, 106, 80, 23, 32, 5, 20, 148, 44, 76, 65, 70, 213, 83, 86, 12, 82, 61, 90, 195, 37, 20, 121, 87, 61, 228, 197, 25, 127, 142, 139, 142, 51, 0, 132, 202, 12, 201, 43, 45, 6, 201, 72, 69, 48, 4, 118, 228, 97, 76, 161, 123, 104, 152, 82, 68, 123, 93, 38, 20, 181, 43, 134, 58, 5, 44, 139, 210, 113, 241, 72, 227, 26, 27, 252, 82, 12, 254, 158, 36, 20, 138, 74, 135, 182, 5, 50, 149, 72, 144, 113, 75, 5, 123, 108, 188, 82, 50, 87, 239, 1, 255, 255, 255, 255, 2, 0, 130, 149, 106, 102, 19, 20, 196, 142, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 98, 239, 1, 255, 255, 255, 255, 2, 0, 130, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
#endif
int newID = 56;
void setup()
{
  Serial.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  mySerial.begin(57600, SERIAL_8N1, 27, 26);
  // set the data rate for the sensor serial port
  //finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println(F("Found !"));
  } else {
    Serial.println(F("not found"));
    while (1) {
      delay(1);
    }
  }
  //finger.emptyDatabase();

  Serial.print("Size "); Serial.println(sizeof(fingerp));
  finger.getTemplateCount();
  Serial.print(F("Sensor contains ")); Serial.print(finger.templateCount); Serial.println(F(" templates"));
  delay(100);
  uint8_t  p = 0;

  Serial.println();
  Serial.println(F("Trying to download the model now"));
  do
  {
    delay(10);
    p = finger.downloadModel(0x01);
    if (p == FINGERPRINT_OK)
    {
      Serial.println("FINGERPRINT_OK");
    }
    if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
      Serial.println("FINGERPRINT_PACKETRECIEVEERR");
    }
  } while (p);
  finger.writeFingerPrintPacket(fingerp);
  //  for (int i = 0; i < 534; i++)
  //  {
  //    mySerial.print(*(fingerp + i));
  //  }
  delay(1000);
  Serial.print("0x01  ");
  Serial.println(p);
  do
  {
    delay(10);
    p = finger.downloadModel(0x02);
    if (p == FINGERPRINT_OK)
    {
      Serial.println("FINGERPRINT_OK");
    }
    if (p == FINGERPRINT_PACKETRECIEVEERR)
    {
      Serial.println("FINGERPRINT_PACKETRECIEVEERR");
    }
  } while (p);
  finger.writeFingerPrintPacket(fingerp);
  //  for (int i = 0; i < 534; i++)
  //  {
  //    mySerial.print(*(fingerp + i));
  //  }
  //delay(1100);
  Serial.print("0x02  ");
  Serial.println(p);
  delay(1000);
  p = finger.createModel();
  Serial.println(p);

  if (p == FINGERPRINT_OK) {
    Serial.println(F("Prints matched!"));
    Serial.print(F("New Fingerprint Template Stored in ")); Serial.println(newID);
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
  delay(1000);

  do
  {
    delay(10);
    p = finger.storeModel(newID);
  } while (p);
  Serial.print("Storing Error : ");
  Serial.println(p);
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
  p = finger.getTemplateCount();
  Serial.println(p);
  Serial.print(F("Sensor contains "));  Serial.print(finger.templateCount);  Serial.println(F(" templates"));
  Serial.println(F("Waiting for valid finger..."));
}

void loop()                     // run over and over again
{
  getFingerprintIDez();
  delay(50);            //don't ned to run this at full speed.
  if (Serial.available() > 0)
  {
    String received = Serial.readString();
    Serial.print("received "); Serial.println(received);
    int n = received.length();
    if (received.startsWith("show"))
    {
      Serial.println("Showing Templates");
      //      for (int finger = 01; finger < count + 1; finger++)
      //      {
      //        downloadFingerprintTemplate(finger);
      //      }
      String Data = received.substring(4, n);
      int id = Data.toInt();
      Serial.print("Showing ID #");
      Serial.println(id);
      downloadFingerprintTemplate(id);
    }
  }
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

  // one data packet is 267 bytes. in one data packet, 11 bytes are 'usesless' :D
  uint8_t bytesReceived[534]; // 2 data packets
  memset(bytesReceived, 0xff, 534);

  uint32_t starttime = millis();
  int i = 0;
  while (i < 534 && (millis() - starttime) < 20000)
  {
    if (mySerial.available())
    {
      bytesReceived[i++] = mySerial.read();
    }
  }
  Serial.print(i); Serial.println(" bytes read.");
  Serial.println("Decoding packet...");

  uint8_t fingerTemplate[534]; // the real template
  memset(fingerTemplate, 0xff, 534);

  // filtering only the data packets
  int uindx = 9, index = 0;
  while (index < 534) 
  {
    while (index < uindx) ++index;
    uindx += 256;
    while (index < uindx)
    {
      fingerTemplate[index++] = bytesReceived[index];
    }
    uindx += 2;
    while (index < uindx) ++index;
    uindx = index + 9;
  }
  for (int i = 0; i < 534; ++i)
  {
    //      Serial.print("0x");
    //      printHex(bytesReceived[i], 2);
    Serial.print(bytesReceived[i]);
    Serial.print(", ");
  }
  Serial.println("\ndone.");

  /*
    uint8_t templateBuffer[256];
    memset(templateBuffer, 0xff, 256);  //zero out template buffer
    int index=0;
    uint32_t starttime = millis();
    while ((index < 256) && ((millis() - starttime) < 1000))
    {
    if (mySerial.available())
    {
      templateBuffer[index] = mySerial.read();
      index++;
    }
    }

    Serial.print(index); Serial.println(" bytes read");

    //dump entire templateBuffer.  This prints out 16 lines of 16 bytes
    for (int count= 0; count < 16; count++)
    {
    for (int i = 0; i < 16; i++)
    {
      Serial.print("0x");
      Serial.print(templateBuffer[count*16+i], HEX);
      Serial.print(", ");
    }
    Serial.println();
    }*/
}


uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image taken"));
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(F("No finger detected"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println(F("Imaging error"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println(F("Image converted"));
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println(F("Image too messy"));
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println(F("Communication error"));
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println(F("Could not find fingerprint features"));
      return p;
    default:
      Serial.println(F("Unknown error"));
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println(F("Found a print match!"));
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println(F("Communication error"));
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println(F("Did not find a match"));
    return p;
  } else {
    Serial.println(F("Unknown error"));
    return p;
  }

  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID);
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print(F("Found ID #")); Serial.print(finger.fingerID);
  Serial.print(F(" with confidence of ")); Serial.println(finger.confidence);
  return finger.fingerID;
}

void printHex(int num, int precision)
{
  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial.print(tmp);
}

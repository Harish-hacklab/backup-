//#define r307
#define r551
//#define r502

#include "SPIFFS.h"
#include "FS.h"
#include <ArduinoJson.h>

#include <HACKLAB_FingerPrint_Library_r30x_r551.h>
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

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
  mySerial.end();
  delay(100);
  mySerial.begin(57600, SERIAL_8N1, 27, 26);
  delay(300);

  if (finger.verifyPassword())
  {
    Serial.print("!!");
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
      deleteFingerprint(id);
    }
    else if (received.startsWith("empty"))
    {
      finger.emptyDatabase();
      Serial.println("Now database is empty :)");
    }
    else if (received == "dislay")
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

      downloadFingerprintTemplate(oldIDee);
      String newID = DATA.substring(comma1p + 1, endp);
      int newIDee = newID.toInt();
      Serial.print("New ID #");
      Serial.println(newIDee);

      uint8_t  p = 0;
      Serial.println(F("\nTrying to download the model now"));
      do
      {
        delay(10);
        p = finger.downloadModel(0x01);
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
        p = finger.storeModel(newIDee);
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
      p = finger.getTemplateCount();
      Serial.println(p);
      Serial.print(F("Sensor contains "));  Serial.print(finger.templateCount);  Serial.println(F(" templates after adding"));
      deleteFingerprint(oldIDee);
      p = finger.getTemplateCount();
      Serial.println(p);
      Serial.print(F("Sensor contains "));  Serial.print(finger.templateCount);  Serial.println(F(" templates after deleting"));
    }
    else
    {
      Serial.println("Unknown Command");
    }
  }
  else
  {
    getFingerprintIDez();
    if (millis() - lastdot > 1000)
    {
      Serial.print(".");
      lastdot = millis();
    }
  }
}

uint8_t getFingerprintEnroll()
{
  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
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
  p = finger.image2Tz(1);
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
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK)
  {
    p = finger.getImage();
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
  p = finger.image2Tz(2);
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
  return 1;
}

void printHex(int num, int precision) {

  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial.print(tmp);
}

uint8_t deleteFingerprint(uint8_t id)
{
  uint8_t p = -1;
  p = finger.deleteModel(id);
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

uint8_t downloadFingerprintTemplate(uint16_t id)
{
  resetFP();
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
  while (i < 534 && (millis() - starttime) < 20000)//here
  {
    if (mySerial.available())
    {
      bytesReceived[i++] = mySerial.read();
    }
  }
  Serial.print(i); Serial.println(" bytes read.");
  Serial.println("Decoding packet...");

  for (int i = 0; i < 534; ++i) //here
  {
    Serial.print("0x");
    printHex(bytesReceived[i], 2);
    Serial.print(", ");
  }
  Serial.println("\ndone.");
  mySerial.end();

  mySerial.begin(57600, SERIAL_8N1, 27, 26);
  if (finger.verifyPassword())
  {
    Serial.print("#");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  resetFP();
}

int getFingerprintIDez()
{

  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  {

    return -1;
  }
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;
  //Serial.println("got characteristics");

#ifdef r307
  p = finger.fingerFastSearch();
#else
  p = finger.fingerSearch();
#endif

  if (p != FINGERPRINT_OK) {
    Serial.println("\n\nNot a Match");
    return -1;
  }
  //Serial.println("search done found");

  // found a match!
  Serial.print("\n\nFound ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}

void deleteFile(fs::FS &fs, const char * path)
{

  //  Serial.println("Deleting file: %s\r\n", path);
  if (fs.remove(path)) {
    Serial.println(F("- file deleted"));
  } else {
    Serial.println(F("- delete failed"));
  }
}

void writeFile(fs::FS & fs, const char * path, String message)
{

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

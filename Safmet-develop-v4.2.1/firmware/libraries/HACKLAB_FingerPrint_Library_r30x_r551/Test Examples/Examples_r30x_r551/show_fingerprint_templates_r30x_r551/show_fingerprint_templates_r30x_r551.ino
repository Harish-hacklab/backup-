/***************************************************
  This is an example sketch for our optical Fingerprint sensor

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <HACKLAB_FingerPrint_Library_r30x_r551.h>
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);


int getFingerprintIDez();
uint8_t downloadFingerprintTemplate(uint16_t id);

// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)



void setup()
{
  while (!Serial);
  Serial.begin(115200);
  Serial.println("Fingerprint template extractor");
  mySerial.begin(57600, SERIAL_8N1, 27, 26);
  // set the data rate for the sensor serial port
  //finger.begin(57600);

  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }

  // Try to get the templates for fingers 1 through 10
  //  for (int finger = 01; finger < 10; finger++)
  //  {
  //    downloadFingerprintTemplate(finger);
  //  }
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

  uint8_t fingerTemplate[512]; // the real template
  memset(fingerTemplate, 0xff, 512);

  // filtering only the data packets
  int uindx = 9, index = 0;
  while (index < 534) {
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
    Serial.print("0x");
    printHex(bytesReceived[i], 2);
    Serial.print(", ");
  }
  Serial.println("\ndone.");
  mySerial.end();

  mySerial.begin(57600, SERIAL_8N1, 27, 26);
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }


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
int i = 1;


void printHex(int num, int precision)
{
  char tmp[16];
  char format[128];

  sprintf(format, "%%.%dX", precision);

  sprintf(tmp, format, num);
  Serial.print(tmp);
}

void loop()
{
  if (i % 2)
  {
    downloadFingerprintTemplate(14);
    delay(2000);
    //    Serial.println("\n\n###################\nend of loop\n###########################\n\n");
  }
  i++;
  Serial.println("\n\n###################\nend of loop\n###########################\n\n");

}

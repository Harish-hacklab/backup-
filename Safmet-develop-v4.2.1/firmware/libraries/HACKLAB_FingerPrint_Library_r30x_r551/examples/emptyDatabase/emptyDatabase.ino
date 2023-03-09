/***************************************************
  This is an example sketch for our optical Fingerprint sensor
  Designed specifically to work with the Adafruit Fingerprint sensor
  ----> http://www.adafruit.com/products/751
  These displays use TTL Serial to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Adafruit_Fingerprint.h>

//SoftwareSerial mySerial(2, 3);
HardwareSerial FingerPrint(2);

//Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FingerPrint);

void setup()
{
  Serial.begin(115200);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  finger.begin(57600);
  if (finger.verifyPassword())
  {
    Serial.println("Found fingerprint sensor!");
  }
  else
  {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }

  Serial.println("\n\nDeleting all fingerprint templates!");
  Serial.println("Press 'Y' key to continue");

  while (1) {
    if (Serial.available() && (Serial.read() == 'Y')) {
      break;
    }
  }

  // set the data rate for the sensor serial port
  finger.emptyDatabase();

  Serial.println("Now database is empty :)");
}

void loop() {
}

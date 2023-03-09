#include <HACKLAB_FingerPrint_Library_r30x_r551.h>

HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()
{
  Serial.begin(115200);
  mySerial.begin(57600, SERIAL_8N1, 27, 26);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  Serial.println("\n\nDelete Finger");

  // set the data rate for the sensor serial port
  //finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
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

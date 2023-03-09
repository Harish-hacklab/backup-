/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/DigitalReadSerial
*/

// digital pin 2 has a pushbutton attached to it. Give it a name:
int enrol_pin = 33;
uint32_t enrol_interrupt_on_time = 0;
bool mode_en = true;
int k;


portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR enrol_interrupt()
{
  portENTER_CRITICAL_ISR(&mux);
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  mode_en = true;
  enrol_interrupt_on_time = millis();
  Serial.println("end of setup");
  last_interrupt_time = interrupt_time;
  portEXIT_CRITICAL_ISR(&mux);
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // make the pushbutton's pin an input:
  pinMode(enrol_pin, INPUT);
  pinMode(23, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(18, OUTPUT);
  digitalWrite(18, LOW);
  digitalWrite(23, LOW);
  digitalWrite(5, LOW);
  attachInterrupt(digitalPinToInterrupt(enrol_pin), enrol_interrupt, FALLING);
}

// the loop routine runs over and over again forever:
void loop()
{
  if (millis() - enrol_interrupt_on_time > 8000 && !digitalRead(enrol_pin) && mode_en) //enrol_timer = 10000
  {
    Serial.println("ENROL!!!!!!!!!!!!!!!!!1");
    digitalWrite(18, HIGH);
    delay(1000);
    digitalWrite(18, LOW);
    mode_en = false;
  }
}

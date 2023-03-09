#define LS_EN 3
#define ATMEGA_GPIO 4


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LS_EN, OUTPUT);
  pinMode(ATMEGA_GPIO, INPUT);
}

void loop() {
  digitalWrite(LS_EN, HIGH);
  int count = 0;
  bool gpio_status = digitalRead(ATMEGA_GPIO);
  if (gpio_status) {
    digitalWrite(LS_EN, LOW);
    delay(1000);
    digitalWrite(LS_EN, HIGH);
    while (!digitalRead(ATMEGA_GPIO)) {
      count++;
      delay(100);
      if (count > 100) {
        count = 0;
        digitalWrite(LS_EN, LOW);
        delay(1000);
        digitalWrite(LS_EN, HIGH);
      }
    }
    delay(1000);
  }
}

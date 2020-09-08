#define LED_PIN 9
#define BLINK_PERIOD_MS 1000

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_PIN, HIGH);   // turn the LED on
  delay(BLINK_PERIOD_MS/2);                       // wait for a second
  digitalWrite(LED_PIN, LOW);    // turn the LED off
  delay(BLINK_PERIOD_MS/2);                       // wait for a second
}

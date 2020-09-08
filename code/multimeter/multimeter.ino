#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  pinMode(A0, INPUT); //set A0 as input to read analog voltage
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay(); // Clear the buffer
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.cp437(true);

  display.write("Darren's\nMultimeter");
  display.display();
  delay(2000);
  display.setTextSize(4);
}

double a0_v;
char voltage[4]; //char array that will hold voltage
void loop() {
  a0_v = analogRead(A0)*5.0/1023.0; //scale to volts
  dtostrf(a0_v,3,2,voltage); //read analog input and convert to char array
  display.clearDisplay();
  display.setCursor(0,0);
  display.write(voltage);
  display.write("v");

  display.display();
  delay(500);

}

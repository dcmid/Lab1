#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define ENC_A 2
#define ENC_B 3

#define PB 5 //digital input pin for pushbutton

#define SPKR 11

volatile int count4x;

enum state_en{
  IDLE,
  TIME,
  ALARM,
  RESET
};

state_en state;

void setup() {
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  pinMode(SPKR, OUTPUT);
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

  display.write("Darren's\nTimer");
  display.display();
  delay(2000);
  display.setTextSize(4);
  state = IDLE;
  count4x = 0;

  attachInterrupt(digitalPinToInterrupt(ENC_A), encoder_handler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), encoder_handler, CHANGE);
}

int count = -1;
int minutes;
int seconds;
char min_char[3];
char sec_char[3];
long start_time;
int elapsed_sec;
void loop(){
  if(state == IDLE){
    // Serial.println("IDLE");
    if(count != count4x/4){
      count = count4x/4;
      display_time(count);
    }
    else if(digitalRead(PB)){ //start timer if button is pressed
      delay(50);
      while(digitalRead(PB));//debounce and wait for release
      delay(50);
      state = TIME;
      start_time = millis();
    }
  }
  else if(state == TIME){
    // Serial.println("TIME");
    int temp_elapsed_sec = (millis() - start_time)/1000;
    if(temp_elapsed_sec != elapsed_sec){
      elapsed_sec = temp_elapsed_sec;
      display_time(count - elapsed_sec);
    }
    else if(count - elapsed_sec == 0){
      count4x = 0;
      analogWrite(SPKR, 128); //start PWM 50% DC output to speaker
      state = ALARM;
    }
  }
  else if(state == ALARM){
    // Serial.println("ALARM");
    if(digitalRead(PB)){ //start timer if button is pressed
      delay(50);
      while(digitalRead(PB));//debounce and wait for release
      delay(50);
      analogWrite(SPKR,0); //turn off speaker
      state = IDLE;
    }
  }
}

void display_time(int cnt){
  minutes = cnt/60;//extract minutes
  seconds = cnt%60;//extract seconds
  //convert ints to character arrays
  min_char[0] = minutes/10 + 48;
  min_char[1] = minutes%10 + 48;
  sec_char[0] = seconds/10 + 48;
  sec_char[1] = seconds%10 + 48;
  // Serial.print(min_char);
  // Serial.print(":");
  // Serial.println(sec_char);

  display.clearDisplay();
  display.setCursor(0,0);
  display.write(min_char);
  display.write(":");
  display.write(sec_char);
  display.display();
}

int read_encoder()
{
  static int enc_states[] = {
    0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0  };
  static byte ABab = 0;
  ABab *= 4;                   //shift the old values over 2 bits
  ABab = ABab%16;      //keeps only bits 0-3
  ABab += 2*digitalRead(ENC_A)+digitalRead(ENC_B); //adds enc_a and enc_b values to bits 1 and 0
  return ( enc_states[ABab]);
}

void encoder_handler()
{
  if(state == IDLE){
    count4x += read_encoder();
    if(count4x < 0)
      count4x = 0;
    else if(count4x > 4*60*99) //max value of 99 minutes
      count4x = 4*60*99;
  }
}

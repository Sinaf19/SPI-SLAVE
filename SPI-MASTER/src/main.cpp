#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>


// MASTER Arduino

// Screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins
#define SCLK_PIN 33
#define MOSI_PIN 32
#define DC_PIN 34
#define CS_PIN 35
#define RST_PIN 36
#define SS2 48

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

#define LED 7
#define button 26

int pot = A8;
int x = 0;
int mode;
int buttonValue;

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);
// Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  tft.begin();
  tft.setRotation(1);

  pinMode(button, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(SS2, OUTPUT);

  digitalWrite(SS, HIGH);
  digitalWrite(SS2, HIGH);
  SPI.setClockDivider(SPI_CLOCK_DIV128);

  tft.fillScreen(BLACK);
  tft.setCursor(0, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Initialisation");
  tft.fillScreen(BLACK);
}

void loop()
{
  // put your main code here, to run repeatedly:
  byte Mastersend, Mastereceive;
  buttonValue = digitalRead(button);
  int potentiometre = analogRead(pot);
  bool ss1 = digitalRead(SS);
  bool ss2 = digitalRead(SS2);

  if (buttonValue)
  {
    x = 1;
    Serial.println("ENVOIE DE X = 1");
  }
  else
  {
    x = 0;
    Serial.println("X = 0");
  }

  digitalWrite(SS, LOW);
  Mastersend = x;
  Mastereceive = SPI.transfer(Mastersend);
  //Serial.println(Mastereceive);

  if (Mastereceive == 85)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }
}
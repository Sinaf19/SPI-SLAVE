#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <SdFat.h>

SdFat SD;
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
#define SS2 44

// Color definitions
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GOLD 0xA508
#define BROWN 0x8200
#define VIOLET 0x9199
#define SILVER 0xA510

#define LED 7
#define button 26
#define button2 40
#define button3 42
#define button4 39
#define LEDSwitch1 3
#define LEDSwitch2 4
#define BlueLED A4
#define chipSelect 22

void buttonFunction();
void SDRead();

volatile byte Mastersend;
volatile byte Mastereceive;
int pot = A8;
int x = 0;
int mode;
int buttonValue1;
int buttonValue2;
int buttonValue3;
int buttonValue4;
int buttonValue4_Mem;
int buttonValue3_Mem;
int ecranT = false;
int ecranH = false;
bool ecranFlagT = true;
bool ecranFlagH = true;
bool ecranFlag = true;
bool onScreen = false;
bool back = false;
bool SDFlag = false;
bool otherSPI = true;
unsigned long texte;

bool rafraichissement = false;
unsigned long interval = 500;
unsigned long rafraichissement_Millis = 0;

Sd2Card card;
FatVolume volume;
SdFile root;
File myFile;

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);
// Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  tft.begin();
  tft.setRotation(1);
  // SD.begin(chipSelect);

  pinMode(chipSelect, OUTPUT);
  pinMode(button, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LEDSwitch1, OUTPUT);
  pinMode(LEDSwitch2, OUTPUT);
  pinMode(SS2, OUTPUT);

  pinMode(MOSI, OUTPUT);
  pinMode(SS, OUTPUT);

  tft.fillScreen(BLACK);
  tft.setCursor(0, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Initialisation");
  tft.fillScreen(BLACK);
  digitalWrite(SS, HIGH);
  digitalWrite(chipSelect, HIGH);
}

void loop()
{
  buttonValue1 = digitalRead(button);
  buttonValue2 = digitalRead(button2);
  buttonValue3 = digitalRead(button3);
  buttonValue4 = digitalRead(button4);

  unsigned long currentMillis = millis();
  if (currentMillis - rafraichissement_Millis >= interval)
  {
    rafraichissement_Millis = currentMillis;
    rafraichissement = !rafraichissement;
  }

  // Début de la communication

  if (otherSPI)
  {
   // delay(100);
    SPI.beginTransaction(SPISettings(9600, MSBFIRST, SPI_MODE0));

    digitalWrite(SS, LOW);

    Mastersend = x;
    Mastereceive = SPI.transfer(Mastersend);
    // Serial.println(Mastereceive);

    /*   if (Mastereceive == 85)
      {
        digitalWrite(LED, HIGH);
      }
      else
      {
        digitalWrite(LED, LOW);
      }
     */

    // Fin de la communication

    // Menu écran OLED et envoie de l'information au slave via la variable X afin de recevoir la température, humidité ou la carte SD
    if ((ecranFlag || buttonValue4) && !back)
    {
      tft.fillScreen(VIOLET);
      tft.setCursor(0, 20);
      tft.setTextColor(WHITE);
      tft.setTextSize(1);
      tft.println(">Bouton 1:Temperature");

      tft.setCursor(0, 50);
      tft.setTextColor(WHITE);
      tft.setTextSize(1);
      tft.println(">Bouton 2: Humidite");

      tft.setCursor(0, 80);
      tft.setTextColor(WHITE);
      tft.setTextSize(1);
      tft.println(">Bouton 3: Carte SD");
      ecranFlag = false;
      onScreen = false;
      back = true;
      ecranH = false;
      ecranT = false;
     // digitalWrite(SS, HIGH);

      x = 0;
    }
    if (buttonValue1 && !onScreen)
    {
      tft.fillScreen(RED);
      tft.setCursor(5, 55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Temperature : ");
      tft.println(Mastereceive);

      tft.setCursor(90, 100);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Retour");
      onScreen = true;
      back = false;
      ecranT = true;

      // Envoie de l'information a l'esclave pour recevoir la température
      x = 1;
    }
    if (rafraichissement and ecranT && Mastereceive > 0)
    {
      //  tft.fillRect(89, 55, 11, 7, RED);
      tft.setCursor(89, 55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE, RED);
      tft.print(Mastereceive);
    }

    if (buttonValue2 && !onScreen)
    {
      tft.fillScreen(BLUE);
      tft.setCursor(5, 55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Humidite : ");
      tft.println(Mastereceive);
      tft.setCursor(85, 55);
      tft.print("%");

      tft.setCursor(90, 100);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Retour");
      onScreen = true;
      back = false;
      ecranH = true;
      // Envoie de l'information a l'esclave pour recevoir l'humiditö
      x = 2;
    }

    if (rafraichissement and ecranH && Mastereceive > 0)
    {
      //  tft.fillRect(70, 55, 13, 7, BLUE);
      tft.setCursor(70, 55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE, BLUE);
      tft.print(Mastereceive);
    }

    if (buttonValue3 && !onScreen)
    {
      tft.fillScreen(GREEN);
      tft.setCursor(5, 55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Envoie d'info de la carte SD");

      tft.setCursor(90, 100);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Retour");
      onScreen = true;
      back = false;

      // SPI.endTransaction();
      otherSPI = false;
      SDFlag = true;

      // Envoie de l'information a l'esclave pour recevoir.... la carte SD
      // x = 3;
    }

    SPI.endTransaction();
    // SPI.end();
    delay(10);
  }

  if (SDFlag)
  {

    digitalWrite(SS, HIGH);
    // Serial.println(digitalRead(SS));
    // delay(100);
    SDRead();
    digitalWrite(chipSelect, HIGH);
    otherSPI = true;
    SDFlag = false;
  }
}

void SDRead()
{

  SD.begin(chipSelect);

  digitalWrite(chipSelect, LOW);

  /*  if (!SD.begin(chipSelect)) {
      Serial.println("initialization failed!");
      return;
    }
    Serial.println("initialization done."); */

  /*  // open the file. note that only one file can be open at a time,
   // so you have to close this one before opening another.
   myFile = SD.open("test.txt", FILE_READ);
    */
  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile)
  {

    // read from the file until there's nothing else in it:
    while (myFile.available())
    {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

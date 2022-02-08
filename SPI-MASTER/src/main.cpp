#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <SD.h>

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

#define LED 7
#define button 26
#define button2 40
#define button3 42
#define button4 39
#define LEDSwitch1 3
#define LEDSwitch2 4
#define BlueLED A4

void buttonFunction();

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
int chgmtEcran;
int chgmtEcranh;
bool ecranFlag = true;
bool ecranFlagh = true;

Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 10;  

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);
// Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  tft.begin();
  tft.setRotation(1);
  
  pinMode(chipSelect, OUTPUT);
  pinMode(button, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(LEDSwitch1, OUTPUT);
  pinMode(LEDSwitch2, OUTPUT);
  pinMode(SS2, OUTPUT);

  Serial.print("\nInitializing SD card...");
  pinMode(SS, OUTPUT);

    while (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
  } 
  
  // print the type of card
  Serial.print("\nCard type: ");
  switch(card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
        // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }


  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("\nVolume type is FAT");
  Serial.println(volume.fatType(), DEC);
  Serial.println();
  
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  Serial.print("Volume size (Kbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Mbytes): ");
  volumesize /= 1024;
  Serial.println(volumesize);

  
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);

     pinMode(MISO, INPUT);
    pinMode(SS, OUTPUT); 

  //  digitalWrite(SS, HIGH);
  //  digitalWrite(SS2, HIGH);
  // SPI.setClockDivider(SPI_CLOCK_DIV128);

   tft.fillScreen(BLACK);
  tft.setCursor(0, 50);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("Initialisation");
  // tft.fillScreen(BLACK); 



}

void loop()
{
      SPI.beginTransaction(SPISettings(9600, MSBFIRST, SPI_MODE0));
    // put your main code here, to run repeatedly:

    buttonValue1 = digitalRead(button);

    buttonValue3 = digitalRead(button3);
    buttonValue4 = digitalRead(button4);
    /* int potentiometre = analogRead(pot);
    bool ss1 = digitalRead(SS);
    bool ss2 = digitalRead(SS2); */

    if (buttonValue1 == HIGH and buttonValue2 == HIGH)
    {
      x = 0;
      digitalWrite(LEDSwitch1, LOW);
      digitalWrite(LEDSwitch2, LOW);
    }
    else if (buttonValue1)
    {
      x = 1;
    }
    else if (buttonValue3)
    {
      x = 3;
      digitalWrite(LEDSwitch2, HIGH);
    }
    else
    {
      x = 0;
      digitalWrite(LEDSwitch1, LOW);
      digitalWrite(LEDSwitch2, LOW);
    }

    if (buttonValue4 != buttonValue4_Mem)
    {

      buttonValue4_Mem = buttonValue4;
      if (buttonValue4 == 1)
      {
        chgmtEcran = 1;
        ecranFlag = true;
        Serial.println("Temperature");
      }
    }

    if (buttonValue4 == 1)
    {
      x = 2;
    }

    if (buttonValue3 != buttonValue3_Mem)
    {

      buttonValue3_Mem = buttonValue3;
      if (buttonValue3 == 1)
      {
        chgmtEcranh = 1;
        ecranFlagh = true;
      }
    }

    digitalWrite(SS, LOW);
    Mastersend = x;
    Mastereceive = SPI.transfer(Mastersend);
    Serial.print("Mastereceive : ");
    Serial.println(Mastereceive);
    Serial.print("Mastersend : ");
    Serial.println(Mastersend);

    if (chgmtEcran == 1 and ecranFlag == true and Mastereceive >= 10)
    {
      tft.fillScreen(RED);
      tft.setCursor(5, 55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Temperature : ");
      Serial.println("Temperature");
      tft.println(Mastereceive);
      ecranFlag = false;
    }
    else if (chgmtEcran == 0 and ecranFlag == true)
    {
      tft.fillScreen(BLACK);
      ecranFlag = false;
    }

    if (chgmtEcranh == 1 and ecranFlagh == true and Mastereceive >= 5)
    {
      tft.fillScreen(BLUE);
      tft.setCursor(5, 55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Humidity : ");
      Serial.println("Humidity");
      tft.println(Mastereceive);
      ecranFlagh = false;
    }
    else if (chgmtEcranh == 0 and ecranFlagh == true)
    {
      tft.fillScreen(BLACK);
      ecranFlagh = false;
    }

    if (Mastereceive == 85)
    {
      digitalWrite(LED, HIGH);
    }
    else
    {
      digitalWrite(LED, LOW);
    }

    SPI.endTransaction();
}



void buttonFunction(){
  buttonValue2 = digitalRead(button2);
  
    digitalWrite(SS2, HIGH);
    SPI.beginTransaction(SPISettings(9600, MSBFIRST, SPI_MODE0));
    // put your main code here, to run repeatedly:

    buttonValue1 = digitalRead(button);

    buttonValue3 = digitalRead(button3);
    buttonValue4 = digitalRead(button4);
    /* int potentiometre = analogRead(pot);
    bool ss1 = digitalRead(SS);
    bool ss2 = digitalRead(SS2); */

    if (buttonValue1 == HIGH and buttonValue2 == HIGH)
    {
      x = 0;
      digitalWrite(LEDSwitch1, LOW);
      digitalWrite(LEDSwitch2, LOW);
    }
    else if (buttonValue1)
    {
      x = 1;
    }
    else if (buttonValue3)
    {
      x = 3;
      digitalWrite(LEDSwitch2, HIGH);
    }
    else
    {
      x = 0;
      digitalWrite(LEDSwitch1, LOW);
      digitalWrite(LEDSwitch2, LOW);
    }

    if (buttonValue4 != buttonValue4_Mem)
    {

      buttonValue4_Mem = buttonValue4;
      if (buttonValue4 == 1)
      {
        chgmtEcran = 1;
        ecranFlag = true;
        Serial.println("Temperature");
      }
    }

    if (buttonValue4 == 1)
    {
      x = 2;
    }

    if (buttonValue3 != buttonValue3_Mem)
    {

      buttonValue3_Mem = buttonValue3;
      if (buttonValue3 == 1)
      {
        chgmtEcranh = 1;
        ecranFlagh = true;
      }
    }

    digitalWrite(SS, LOW);
    Mastersend = x;
    Mastereceive = SPI.transfer(Mastersend);
    Serial.print("Mastereceive : ");
    Serial.println(Mastereceive);
    Serial.print("Mastersend : ");
    Serial.println(Mastersend);

    if (chgmtEcran == 1 and ecranFlag == true and Mastereceive >= 10)
    {
      tft.fillScreen(RED);
      tft.setCursor(5, 55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Temperature : ");
      Serial.println("Temperature");
      tft.println(Mastereceive);
      ecranFlag = false;
    }
    else if (chgmtEcran == 0 and ecranFlag == true)
    {
      tft.fillScreen(BLACK);
      ecranFlag = false;
    }

    if (chgmtEcranh == 1 and ecranFlagh == true and Mastereceive >= 5)
    {
      tft.fillScreen(BLUE);
      tft.setCursor(5, 55);
      tft.setTextSize(1);
      tft.setTextColor(WHITE);
      tft.print("Humidity : ");
      Serial.println("Humidity");
      tft.println(Mastereceive);
      ecranFlagh = false;
    }
    else if (chgmtEcranh == 0 and ecranFlagh == true)
    {
      tft.fillScreen(BLACK);
      ecranFlagh = false;
    }

    if (Mastereceive == 85)
    {
      digitalWrite(LED, HIGH);
    }
    else
    {
      digitalWrite(LED, LOW);
    }

    SPI.endTransaction();
  

/*   if (buttonValue2 == 0)
  {

    digitalWrite(SS, HIGH);
    digitalWrite(SS2, LOW);
    Serial.println("In the matrix");
    SPI.beginTransaction(SPISettings(9600, MSBFIRST, SPI_MODE0));

    Mastersend = 30;

    Mastersend = SPI.transfer(Mastereceive);
    Serial.println(Mastereceive);
    analogWrite(BlueLED, Mastereceive);

    SPI.endTransaction();
  } */
}
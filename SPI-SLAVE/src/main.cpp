#include <Arduino.h>
#include <SPI.h>
#include <dht.h>
#include <Adafruit_Sensor.h>

// SLAVE code
#define LED 7
#define button 22
#define button2
DHT dht(4, DHT11);

int buttonValue;
volatile byte Slavereceived;
volatile byte Slavesend;
int data;
int humidity;

void setup()
{
  // Setup pour que le SPI fonctionne correctement avec slmt un bouton qui envoie info au master et une led pour dire que l'info du master a bien été reçue
  Serial.begin(9600);
  SPI.begin();
  pinMode(LED, OUTPUT);
  pinMode(button, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT_PULLUP);
  dht.begin();
}

void loop()
{
  // Reçoit et update la valeur pour envoyer au MASTER et allumer ou éteindre la LED pour informer de la bonne réception des données
  SPI.beginTransaction(SPISettings(100, MSBFIRST, SPI_MODE0));
  buttonValue = digitalRead(button);

//Si le bouton est appuyé envoie 85 au MASTER
  if (buttonValue)
  {
    data = 85;
  }
  else
  {
    data = 0;
  }
  //Envoie des données et réception des data du MASTER
  Slavesend = data;
  Slavereceived = SPI.transfer(Slavesend);

  
/*   Serial.println(Slavesend);
  Serial.println(Slavereceived); */

//Si le data du MASTER est de 1 --> allume une LED
  if (Slavereceived == 1)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }

  //Si reçoit un 2 du MASTER envoie la température
  if (Slavereceived == 2)
  {
    data = dht.readTemperature();
    Serial.print("Température :");
    Serial.println(data);
  }
  else
  {
    data = 0;
  }

//Envoie la température
  Slavesend = data;
  Slavereceived = SPI.transfer(Slavesend);

//Si reçoit 3 du MASTER envoie l'humidité
  if (Slavereceived == 3)
  {
    data = dht.readHumidity();
    Serial.print("Humidité :");
    Serial.println(data);
  }
  else
  {
    data = 0;
  }
//Envoie l'humiodité
  Slavesend = data;
  Slavereceived = SPI.transfer(Slavesend);

  Serial.println(Slavereceived);
  SPI.endTransaction();








}

// Librairies utilisées
#include <Arduino.h>
#include <SPI.h>
#include <dht.h>
#include <Adafruit_Sensor.h>
#include <SD.h>

// SLAVE code
// Définition des éléments physiques
#define LEDR 6
#define LEDB 7
#define LEDV 8
#define button 22
#define button2
DHT dht(4, DHT11);

// Définition des variables
int buttonValue;
volatile byte Slavereceived;
volatile byte Slavesend;
int data;
int datat;
int datah;
int humidity;
int temperature;


void setup()
{
  // Setup pour que le SPI fonctionne correctement avec slmt un bouton qui envoie info au master et une led pour dire que l'info du master a bien été reçue
  Serial.begin(9600);
  SPI.begin();
  pinMode(LEDR, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDV, OUTPUT);
  pinMode(button, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT_PULLUP);
//  SPCR |= _BV(SPE);
  dht.begin();
}

void loop()
{
  // Reçoit et update la valeur pour envoyer au MASTER et allumer ou éteindre la LED pour informer de la bonne réception des données
  SPI.beginTransaction(SPISettings(9600, MSBFIRST, SPI_MODE0));
  buttonValue = digitalRead(button);
  Slavereceived = SPI.transfer(Slavesend);
  // Si le bouton est appuyé envoie 85 au MASTER
/*   if (buttonValue)
  {
    data = 85;
    //  Serial.println(data);
  }
  else
  {
    data = 0;
  }
  // Envoie des données et réception des data du MASTER
  Slavesend = data;
  Slavereceived = SPI.transfer(Slavesend); */
  /*   Serial.print(Slavesend); */

  // Si le data du MASTER est de 1 --> allume une LED
  
  if (Slavereceived == 0)
  {
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDV, LOW);
    digitalWrite(LEDB, LOW);
    datat = 0;
    datah = 0;
    temperature = false;
    humidity = false;
  } 
  if (Slavereceived == 1)
  {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDV, LOW);
    temperature = true;
    humidity = false;
    
  }
  else if (Slavereceived == 2)
  {
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDV, LOW);
    temperature = false;
    humidity = true;
  }
  else if (Slavereceived == 3)
  {
    digitalWrite(LEDV, HIGH);
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDB, LOW);
  }


  // Serial.print(Slavereceived);
  
    // Si reçoit un 2 du MASTER envoie la température
    if (temperature)
    {
      datat = dht.readTemperature();
    //  Serial.print("Température :");
      Serial.println(datat);
    } else {
      datat = 0;
      temperature = 0;
    }
    

    
    // Envoie la température
    if (datat != 0)
    {
      Slavesend = datat;
      SPI.transfer(Slavesend);
      
    }
    // Si reçoit 3 du MASTER envoie l'humidité
     if (humidity)
    {
      datah = dht.readHumidity();
     // Serial.print("Humidité :");
      Serial.println(datah);
    }
    else
    {
      datah = 0;
      humidity = 0;
    }

    // Envoie l'humiodité
    if (datah != 0)
    {
      Slavesend = datah;
      SPI.transfer(Slavesend);
    }
   
  
  SPI.endTransaction();
  
  
}
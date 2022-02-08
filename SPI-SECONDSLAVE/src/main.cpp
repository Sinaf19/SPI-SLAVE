
// Librairies utilisées
#include <Arduino.h>
#include <SPI.h>

// SLAVE code
// Définition des éléments physiques
#define LED 7
#define button 22
#define button2
#define potentiometer A7

// Définition des variables
int buttonValue;
volatile byte Slavereceived;
volatile byte Slavesend;
int potValue;

void setup()
{
  // Setup pour que le SPI fonctionne correctement avec slmt un bouton qui envoie info au master et une led pour dire que l'info du master a bien été reçue
  Serial.begin(9600);
  SPI.begin();
  pinMode(LED, OUTPUT);
  pinMode(button, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT_PULLUP);
  pinMode(potentiometer, INPUT);
  
}

void loop()
{


    // Reçoit et update la valeur pour envoyer au MASTER et allumer ou éteindre la LED pour informer de la bonne réception des données
    SPI.beginTransaction(SPISettings(9600, MSBFIRST, SPI_MODE0));
    potValue = analogRead(potentiometer);
    potValue = potValue / 4;
    Slavesend = potValue;
    Slavereceived = SPI.transfer(Slavesend);
    Serial.println(Slavesend);
    Serial.println(digitalRead(MISO));

    SPI.endTransaction();

}
#include <Arduino.h>
#include <SPI.h>

// SLAVE code
#define LED 7
#define button 22

int buttonValue;
volatile byte Slavereceived;
volatile byte Slavesend;
int data;

void setup()
{
  // Setup pour que le SPI fonctionne correctement avec slmt un bouton qui envoie info au master et une led pour dire que l'info du master a bien été reçue
  Serial.begin(9600);
  SPI.begin();
  pinMode(LED, OUTPUT);
  pinMode(button, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT_PULLUP);
}

void loop()
{
  // Reçoit et update la valeur pour envoyer au MASTER et allumer ou éteindre la LED pour informer de la bonne réception des données
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));

  buttonValue = digitalRead(button);

  if (buttonValue)
  {
    data = 85;
  }
  else
  {
    data = 0;
  }

  Slavesend = data;
  Slavereceived = SPI.transfer(Slavesend);

  Serial.println(MISO);
  Serial.println(Slavesend);
  // Serial.println(Slavereceived);

  if (Slavereceived == 1)
  {
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(LED, LOW);
  }

  SPI.endTransaction();
}
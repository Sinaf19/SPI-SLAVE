#include <Arduino.h>
#include <SPI.h>




//SLAVE code
#define LED 26 




void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  SPI.begin();


}

void loop() {
  // put your main code here, to run repeatedly:
}
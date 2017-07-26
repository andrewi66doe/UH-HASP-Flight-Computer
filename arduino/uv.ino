#include "sma.h"

/* Last update: 7/18/2017 */

SMAFilter sma0(5, pVolt0);                  // Diode to PIN 0
SMAFilter sma1(5, pVolt1);                  // Diode to PIN 1
SMAFilter sma2(5, pVolt2);                  // Diode to PIN 2

/*  Read each Diode */
float pVolt0() {
  int pinRead0 = analogRead(0);
  float volt = pinRead0 / 1024.0 * 5.0;
  return (volt);
}

float pVolt1() {
  int pinRead0 = analogRead(1);
  float volt = pinRead0 / 1024.0 * 5.0;
  return (volt);
}


float pVolt2() {
  int pinRead0 = analogRead(2);
  float volt = pinRead0 / 1024.0 * 5.0;
  return (volt);
}


void setup() {
  Serial.begin(9600);
}


void loop() {
  /* Read and Print Delimited by "," and new line */
  Serial.print(sma0.getFilteredSample());
  Serial.print(" , ");
  Serial.print(sma1.getFilteredSample());
  Serial.print(" , ");
  Serial.print(sma2.getFilteredSample());
  Serial.println("");   
  delay(100);
}

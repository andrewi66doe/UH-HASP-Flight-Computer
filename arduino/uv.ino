#include "sma.h"

float pVolt0() { return float(analogRead(0) / 1024.0 * 5.0); }
float pVolt1() { return float(analogRead(1) / 1024.0 * 5.0); }
float pVolt2() { return float(analogRead(2) / 1024.0 * 5.0); }

SMAFilter sma0(5, pVolt0);
SMAFilter sma1(5, pVolt1);
SMAFilter sma2(5, pVolt2);

void setup() { 
  Serial.begin(9600); 
}


void loop() {
  // Readings in mW/m^2
  Serial.print(pVolt0() / 1013000000.0 / (0.12 *0.22) *1000000000.0);
  Serial.print(", ");
  Serial.print(pVolt1()/ 1013000000.0 / (0.12 *0.22) *1000000000.0);
  Serial.print(", ");
  Serial.println(pVolt2()/ 1013000000.0 / (0.12 *0.22) *1000000000.0);
  
  delay(100);
}

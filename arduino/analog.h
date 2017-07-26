/* Code pertaining to analog devices, i.e the UV and temperature sensors */

#include "SparkFun_HIH4030.h"

#define HIH4030_OUT 10

// Supply Voltage - Typically 5 V
#define HIH4030_SUPPLY 5.00


HIH4030 humiditySensor(HIH4030_OUT, HIH4030_SUPPLY);


int averageAnalogRead(int pinToRead);

float readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return 5.0;
}

String readTemp(int pin)
{
  float reading = analogRead(pin);
  float voltage = reading * 5.2;
  voltage /= 1024.0;

  float temp_c = (voltage - 0.5) * 100 ;
  float temp_f = (temp_c * 9.0 / 5.0) + 32.0;
  return String(temp_f);  
}

String readHumidityReal(int pin)
{
  float temp = pressure_sensor.getTemperature(CELSIUS, ADC_4096);
  float humidity = humiditySensor.getTrueRH(temp);
  return String(humidity);
}


float readUV(int pin)
{
  float reading = analogRead(pin);
  float volts = reading/1024.0 * 5.0;

  return volts;
  
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}


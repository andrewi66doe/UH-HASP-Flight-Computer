/* Code pertaining to analog devices, i.e the UV and temperature sensors */

float getVcc();

String readTemp(int pin)
{
  float reading = analogRead(pin);
  float voltage = reading * 1.1;
  voltage /= 1023.0;

  float temp_c = (voltage - 0.5) * 100 ;
  float temp_f = (temp_c * 9.0 / 5.0) + 32.0;
  //float voltage = analogRead(device_id) * (5000/1024);
  //float temp = (((voltage - .5) * 100) * 1.8) + 32;
  return String(temp_f);  
}

String readHumidity(int pin)
{
  float voltage = analogRead(pin) * 5 / 1023;
  float relative_humidity = (voltage / (.0062 * 5)) - 25.81;
  float true_humidity = relative_humidity/(1.0546 - (0.00216 * pressure_sensor.getTemperature(FAHRENHEIT, ADC_2048)));
  return String(true_humidity);
}

String readUV(int pin)
{
  float reading = analogRead(pin);
  float volts = reading / 1024.0 * 5;




  Serial.print("[" + timeStamp() + "] " + "[DATA] PHOTO_" + pin +": ");
  Serial.print(volts, 7);
  Serial.print('\n');

  return String(volts);
  
}

float getVcc() { // Returns actual value of Vcc (in milliVolts), modified code copied from the forum
  const int32_t InternalReferenceVoltage = 1100; // assume perfect 1.1V reference voltage, the absolute error does not matter much
  // REFS1 REFS0          --> 0 1, AVcc internal ref. -Selects AVcc external reference
  // MUX3 MUX2 MUX1 MUX0  --> 1110 1.1V (VBG)         -Selects channel 14, bandgap voltage, to measure
  ADMUX = _BV (REFS0) | _BV (MUX3) | _BV (MUX2) | _BV (MUX1);
  delay(2); // this seems necessary for a stable reading
  ADCSRA |= _BV( ADSC ); // Start a conversion
  while ((ADCSRA & _BV(ADSC))); // Wait for it to complete
  return (float)(((InternalReferenceVoltage * 1024L) / ADC) + 5L); // Scale and round the value
}


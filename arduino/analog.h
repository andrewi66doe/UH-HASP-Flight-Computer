/* Code pertaining to analog devices, i.e the UV and temperature sensors */

String readTemp(int pin)
{
  float reading = analogRead(pin);
  float voltage = reading * 5.0;
  voltage /= 1024.0; 
 
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
  float true_humidity = relative_humidity/(1.0546 - (0.00216 * (readTemp(TEMP_0)).toFloat()));
  return String(true_humidity);
}



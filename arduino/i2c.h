/* Code pertaining to i2c devices i,e the gyro and pressure sensors */
#include "SparkFun_MS5803_I2C.h"

// Base line altitude for Houston TX, for testing purposes only don't touch otherwise
static double BASE_ALTITUDE = 80.0;

MS5803 pressure_sensor(ADDRESS_HIGH);

// Shamelessly stolen code below

 double sealevel(double P, double A)
// Given a pressure P (mbar) taken at a specific altitude (meters),
// return the equivalent pressure (mbar) at sea level.
// This produces pressure readings that can be used for weather measurements.
{
  return(P/pow(1-(A/44330.0),5.255));
}


double altitude(double P, double P0)
// Given a pressure measurement P (mbar) and the pressure at a baseline P0 (mbar),
// return altitude (meters) above baseline.
{
  return(44330.0*(1-pow(P/P0,1/5.255)));
}
// End shamelessly stolen code

String getPressure()
{
  return String(pressure_sensor.getPressure(ADC_512));
}

String getTemperature()
{
  return String(pressure_sensor.getTemperature(FAHRENHEIT, ADC_512));
}

String getRelativePressure(double baseline)
{
  float pressure_abs = pressure_sensor.getPressure(ADC_4096);
  return String(sealevel(pressure_abs, baseline));
}



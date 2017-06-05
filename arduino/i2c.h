/* Code pertaining to i2c devices i,e the gyro and pressure sensors */
// Gyro module library
#include "SparkFun_MS5803_I2C.h"
// RTC library
#include "SparkFunDS3234RTC.h"

#include <Wire.h>

#define    MPU9250_ADDRESS            0x68
#define    MAG_ADDRESS                0x0C

#define    AK8963_WHO_AM_I            0x00
#define    AK8963_CNTL                0x0A
#define    MAG_STATUS                 0x02

#define    GYRO_FULL_SCALE_250_DPS    0x00  
#define    GYRO_FULL_SCALE_500_DPS    0x08
#define    GYRO_FULL_SCALE_1000_DPS   0x10
#define    GYRO_FULL_SCALE_2000_DPS   0x18

#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

#define SMA_ITERATIONS 5

// Base line altitude for Houston TX, for testing purposes only don't touch otherwise
static double BASE_ALTITUDE = 80.0;

MS5803 pressure_sensor(ADDRESS_HIGH);

// Filtered accelerometer values 
float ax_f=0, ay_f=0, az_f=0;
long int iter=0;

// Arrays to store data points for SMA filter
int16_t x_vals[SMA_ITERATIONS];
int16_t y_vals[SMA_ITERATIONS];
int16_t z_vals[SMA_ITERATIONS];

// Sums for calculating averages for each accelerometer axis
int16_t sumx,sumy,sumz;


// This function read Nbytes bytes from I2C device at address Address. 
// Put read bytes starting at register Register in the Data array. 
void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  
  // Read Nbytes
  Wire.requestFrom(Address, Nbytes); 
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
  delay(40);
}


// Write a byte (Data) in device (Address) at register (Register)
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data)
{
  // Set register address
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
  delay(40);
}

void initGyro()
{
  uint8_t WHOAMI;
  
  I2Cread(MAG_ADDRESS, AK8963_WHO_AM_I, 1, &WHOAMI);
  
  if(WHOAMI != 0x48){
    error("Gyroscope Identification Register is invalid!");
    error("Failed to initialize gyroscope.");
    return;
  }
  
  I2CwriteByte(MAG_ADDRESS, AK8963_CNTL, 0x00);
  delay(10);
  I2CwriteByte(MAG_ADDRESS, AK8963_CNTL, 0x0F);
  delay(10);
  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_2000_DPS); // Configure gyroscope range
  delay(10);
  I2CwriteByte(MAG_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer  
  delay(10);
  I2CwriteByte(MAG_ADDRESS, AK8963_CNTL, 0x06);
  delay(10);
  I2CwriteByte(MAG_ADDRESS,0x0A,0x01); // Request first response from gyro
  delay(10);
}

void AccelerometerRead(){
  uint8_t Buf[6];
  // Read Accelerometer values from registers
  I2Cread(MPU9250_ADDRESS,0x3B,6,Buf);
  
  // Convert hi and low order bits of accelerometer registers to full 16-bit values
  // Accelerometer
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t az=Buf[4]<<8 | Buf[5];

  // Need to take 5 readings before we can maintain a filtered data point
  if(iter <= SMA_ITERATIONS){
    x_vals[iter] = ax;
    y_vals[iter] = ay;
    z_vals[iter] = az;
  }else{
    sumx=0;sumy=0;sumz=0;
    // Calculate moving average
    for(int i=0;i<SMA_ITERATIONS;i++){
      sumx += x_vals[i];
      sumy += y_vals[i];
      sumz += z_vals[i];
      ax_f = sumx/SMA_ITERATIONS;
      ay_f = sumy/SMA_ITERATIONS;
      az_f = sumz/SMA_ITERATIONS;
    }
    // Shift values left
    for(int i=1;i<SMA_ITERATIONS;i++){
      x_vals[i-1] = x_vals[i];
      y_vals[i-1] = y_vals[i];
      z_vals[i-1] = z_vals[i];
    }
    x_vals[SMA_ITERATIONS-1] = ax;
    y_vals[SMA_ITERATIONS-1] = ay;
    z_vals[SMA_ITERATIONS-1] = az;
  }
  send_data(ACCEL_X, String(ax_f));
  send_data(ACCEL_Y, String(ay_f));
  send_data(ACCEL_Z, String(az_f));
  iter++;
  
}



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



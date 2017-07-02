/* Code pertaining to i2c devices i,e the gyro and pressure sensors */
// Gyro module library
#include "SparkFun_MS5803_I2C.h"
// RTC library
#include "SparkFunDS3234RTC.h"

#include <EEPROM.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

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


#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55);
MS5803 pressure_sensor(ADDRESS_HIGH);


// Base line altitude for Houston TX, for testing purposes only don't touch otherwise
static double BASE_ALTITUDE = 80.0;


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

void readBNO055()
{
  sensors_event_t event;
  bno.getEvent(&event);
  
  send_data(ACCEL_X, String(event.orientation.x));
//  Serial.println(String(event.orientation.x) + " " + String(event.orientation.y) + " " + String(event.orientation.z));
  send_data(ACCEL_Y, String(event.orientation.y));
  send_data(ACCEL_Z, String(event.orientation.z));
  
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

void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
*/
/**************************************************************************/
void displaySensorStatus(void)
{
  /* Get the system status values (mostly for debugging purposes) */
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  /* Display the results in the Serial Monitor */
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0x");
  Serial.println(self_test_results, HEX);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display sensor calibration status
*/
/**************************************************************************/
void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  LCD.write(12);

    /* Display the individual values */
  LCD.print("Sys:");
  LCD.print(system, DEC);
  LCD.print(" G:");
  LCD.print(gyro, DEC);
  LCD.print(" A:");
  LCD.print(accel, DEC);
  LCD.print(" M:");
  LCD.print(mag, DEC);
  LCD.println();
  delay(20);
  
}


void setupBNO055()
{
  Serial.println("Orientation Sensor Test"); Serial.println("");

    /* Initialise the sensor */
    if (!bno.begin())
    {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }

    int eeAddress = 0;
    long bnoID;
    bool foundCalib = false;

    EEPROM.get(eeAddress, bnoID);

    adafruit_bno055_offsets_t calibrationData;
    sensor_t sensor;

    /*
    *  Look for the sensor's unique ID at the beginning oF EEPROM.
    *  This isn't foolproof, but it's better than nothing.
    */
    bno.getSensor(&sensor);
    if (bnoID != sensor.sensor_id)
    {
        Serial.println("\nNo Calibration Data for this sensor exists in EEPROM");
        delay(500);
    }
    else
    {
        Serial.println("\nFound Calibration for this sensor in EEPROM.");
        eeAddress += sizeof(long);
        EEPROM.get(eeAddress, calibrationData);

//        displaySensorOffsets(calibrationData);

        Serial.println("\n\nRestoring Calibration data to the BNO055...");
        bno.setSensorOffsets(calibrationData);

        Serial.println("\n\nCalibration data loaded into BNO055");
        foundCalib = true;
    }

    delay(1000);

    /* Display some basic information on this sensor */
    displaySensorDetails();

    /* Optional: Display current status */
    displaySensorStatus();

    bno.setExtCrystalUse(true);

    sensors_event_t event;
    bno.getEvent(&event);
    if (foundCalib){
        Serial.println("Move sensor slightly to calibrate magnetometers");
        while (!bno.isFullyCalibrated())
        {
            displayCalStatus();
            bno.getEvent(&event);
            delay(500);
        }
        LCD.write(12);
        LCD.print("Calibrated!");
    }
    else
    {
        Serial.println("Please Calibrate Sensor: ");
        while (!bno.isFullyCalibrated())
        {
            bno.getEvent(&event);

            Serial.print("X: ");
            Serial.print(event.orientation.x, 4);
            Serial.print("\tY: ");
            Serial.print(event.orientation.y, 4);
            Serial.print("\tZ: ");
            Serial.print(event.orientation.z, 4);

            /* Optional: Display calibration status */
            displayCalStatus();

            /* New line for the next sample */
            Serial.println("");

            /* Wait the specified delay before requesting new data */
            delay(BNO055_SAMPLERATE_DELAY_MS);
        }
    }

    Serial.println("\nFully calibrated!");
    Serial.println("--------------------------------");
    Serial.println("Calibration Results: ");
    adafruit_bno055_offsets_t newCalib;
    bno.getSensorOffsets(newCalib);
//    displaySensorOffsets(newCalib);

    Serial.println("\n\nStoring calibration data to EEPROM...");

    eeAddress = 0;
    bno.getSensor(&sensor);
    bnoID = sensor.sensor_id;

    EEPROM.put(eeAddress, bnoID);

    eeAddress += sizeof(long);
    EEPROM.put(eeAddress, newCalib);
    Serial.println("Data stored to EEPROM.");

    Serial.println("\n--------------------------------\n");
    delay(500);

}

void setupRTC()
{
  rtc.begin(DS13074_CS_PIN);
  rtc.autoTime();
  digitalWrite(RTC_STATUS, HIGH);
}

void setupMS5803()
{
  pressure_sensor.reset();
  pressure_sensor.begin();
  digitalWrite(MS5803_STATUS, HIGH);
}



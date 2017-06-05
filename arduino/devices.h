#include "SparkFunDS3234RTC.h"

#define TEMP_INTERVAL     2000
#define PRESSURE_INTERVAL 2000
#define UV_INTERVAL    2000
#define TEMP_INTERVAL     2000
#define HUMIDITY_INTERVAL 2000

enum Device{
  // Analog devices
  TEMP_0,
  TEMP_1,
  TEMP_2, 
  TEMP_3,
  TEMP_4,
  TEMP_5,
  TEMP_6,
  HUMIDITY,
  PHOTO_0,
  PHOTO_1,
  PHOTO_2,
  PRESSURE_S,

  // I2C devices
  GYRO,
  ACCEL_X,
  ACCEL_Y,
  ACCEL_Z,
  MAGNO,
  IDK,
  YAW,
  PITCH,
  ROLL,
  RATE,

  // Digital devices
  CLK
};

String timeStamp();


void send_data(Device dev, String data)
{
  String device_str;
  
  switch(dev) {
    case TEMP_0:
      device_str = "TEMP_0: ";
      break;
    case TEMP_1:
      device_str = "TEMP_1: ";
      break;
    case TEMP_2:
      device_str = "TEMP_2: ";
      break;
    case TEMP_3:
      device_str = "TEMP_3: ";
      break;
    case TEMP_4:
      device_str = "TEMP_4: ";
      break;
    case TEMP_5:
      device_str = "TEMP_5: ";
      break;
    case TEMP_6:
      device_str = "TEMP_6: ";
      break;
    case PHOTO_0:
      device_str = "PHOTO_0: ";
      break;
    case PHOTO_1:
      device_str = "PHOTO_1: ";
      break;
    case PHOTO_2:
      device_str = "PHOTO_2: ";
      break;
    case ACCEL_X:
      device_str = "ACCEL_X: ";
      break;
    case ACCEL_Y:
      device_str = "ACCEL_Y: ";
      break;
    case ACCEL_Z:
      device_str = "ACCEL_Z: ";
      break;
    case GYRO:
      device_str = "GYRO: ";
      break;
    case MAGNO:
      device_str = "MAGNO: ";
      break;
    case IDK:
      device_str = "IDK: ";
      break;
    case YAW:
      device_str = "YAW: ";
      break; 
    case PITCH:
      device_str = "PITCH: ";
      break; 
    case ROLL:
      device_str = "ROLL: ";
      break;
    case RATE:
      device_str = "RATE: ";
      break;
    case PRESSURE_S:
      device_str = "PRESSURE_S: ";
      break;
   case HUMIDITY:
      device_str = "HUMIDITY: ";
      break;
   case CLK:
      device_str = "CLK: ";
      break;
    
  }
  
  Serial.println("[" + timeStamp() + "] " + "[DATA] " + device_str + data); 
}


String timeStamp()
{
  String timestamp = String(rtc.hour()) + ":";
  
  if (rtc.minute() < 10)
    timestamp = timestamp + "0";
  timestamp = timestamp + String(rtc.minute()) + ":"; // Print minute
  if (rtc.second() < 10)
    timestamp = timestamp + "0";
  timestamp = timestamp + String(rtc.second()) + " "; // Print second
  

  if (rtc.is12Hour()) // If we're in 12-hour mode
  {
    // Use rtc.pm() to read the AM/PM state of the hour
    if (rtc.pm()) Serial.print(" PM"); // Returns true if PM
    else Serial.print(" AM");
  }
  
  //Serial.print(" | ");

  // Few options for printing the day, pick one:
  //Serial.print(rtc.dayStr()); // Print day string
  //Serial.print(rtc.dayC()); // Print day character
  //Serial.print(rtc.day()); // Print day integer (1-7, Sun-Sat)
  timestamp = timestamp + String(rtc.month()) + "/" + String(rtc.date()) + "/" + String(rtc.year());

  return timestamp;
}


#include "SparkFunDS3234RTC.h"

#define TEMP_INTERVAL     3000 // 3 seconds
#define PRESSURE_INTERVAL 3500 // 3.5 seconds
#define UV_INTERVAL       3000
#define TEMP_INTERVAL     3000
#define HUMIDITY_INTERVAL 3000
#define GPS_INTERVAL      15000 // 15 seconds

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
  ACCEL,
  BNO055_TEMP,
  MAG,
  IDK,
  HEADING,

  // Digital devices
  CLK,
  GEOPOSITION,
  GPS_TIME,
  SAT_COUNT,
  ALTITUDE
  
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
    case ACCEL:
      device_str = "ACCEL: ";
      break;
    case GYRO:
      device_str = "GYRO: ";
      break;
    case MAG:
      device_str = "MAGNO: ";
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
   case BNO055_TEMP:
      device_str = "BNO055_TEMP: ";
      break;
   case HEADING:
    device_str = "HEADING: ";
    break;
   case GEOPOSITION:
    device_str = "GEOPOSITION: ";
    break;
   case GPS_TIME:
    device_str = "GPS_TIME: ";
    break;
   case SAT_COUNT:
    device_str = "SAT_COUNT: ";
    break;
   case ALTITUDE:
    device_str = "ALTITUDE: ";
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
  
  //Serial.print(" | ");

  // Few options for printing the day, pick one:
  //Serial.print(rtc.dayStr()); // Print day string
  //Serial.print(rtc.dayC()); // Print day character
  //Serial.print(rtc.day()); // Print day integer (1-7, Sun-Sat)
  timestamp = timestamp + String(rtc.month()) + "/" + String(rtc.date()) + "/" + String(rtc.year());

  return timestamp;
}


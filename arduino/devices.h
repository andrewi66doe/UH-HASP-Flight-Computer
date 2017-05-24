// Analog devices
#define TEMP_0 1
#define TEMP_1 2
#define TEMP_2 3
#define TEMP_3 4
#define TEMP_4 5
#define TEMP_5 6
#define TEMP_6 7
#define HUMIDITY 8
#define PHOTO_0 9
#define PHOTO_1 10
#define PHOTO_2 11

// I2C devices
#define GYRO 12
#define PRESSURE_S 13

// Digital devices
#define CLK 14

String timeStamp();


void send_data(char device_id, String data)
{
  String device_str;
  
  switch(device_id) {
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
    case GYRO:
      device_str = "GYRO: ";
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


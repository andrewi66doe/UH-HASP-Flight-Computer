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
#define PRESSURE 13

// Digital devices
#define CLK 14


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
    case PRESSURE:
      device_str = "PRESSURE: ";
      break;
   case CLK:
      device_str = "CLK: ";
      break;
    
  }
  Serial.println("[DATA] " + device_str + data); 
}



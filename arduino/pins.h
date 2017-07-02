// Analog pins
#define PHOTO_0_PIN 0
#define PHOTO_1_PIN 1
#define PHOTO_2_PIN 2

#define TEMP_0_PIN 3
#define TEMP_1_PIN 4
#define TEMP_2_PIN 5
#define TEMP_3_PIN 6
#define TEMP_4_PIN 7
#define TEMP_5_PIN 8
#define TEMP_6_PIN 9

#define HUMIDITY_PIN 10

// RTC pins

#define RTC_SS 53
#define RTC_MOSI 51
#define RTC_MISO 50

// I2C

#define I2C_SCL 21
#define I2C_SDA 20
#define DS13074_CS_PIN 53

// LED Status Pins
#define RTC_STATUS         2
#define MS5803_STATUS      4
#define BNO055_STATUS      6

void setupPins()
{
  pinMode(RTC_STATUS, OUTPUT);
  pinMode(MS5803_STATUS, OUTPUT);
  pinMode(BNO055_STATUS, OUTPUT);
}



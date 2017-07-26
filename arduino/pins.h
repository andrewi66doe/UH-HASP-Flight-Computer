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

#define RTC_SS   53
#define RTC_MOSI 51
#define RTC_MISO 50

// I2C

#define I2C_SCL 21
#define I2C_SDA 20
#define DS13074_CS_PIN 53

// LED Status Pins
#define RTC_STATUS         2
#define DOWNLINK_STATUS    3
#define MS5803_STATUS      4
#define BNO055_STATUS      6


// Relay pins
#define SOLENOID_RELAY_PIN  24
#define UNDEFINED_RELAY_PIN 26
#define PUMP_RELAY_PIN      28
#define HEATER_RELAY_PIN    30

// HASP Discrete interface
#define HASP_DISCRETE_1 36
#define HASP_DISCRETE_2 38
#define HASP_DISCRETE_3 40
#define HASP_DISCRETE_4 42


void setupPins()
{
  pinMode(SOLENOID_RELAY_PIN, OUTPUT);
  pinMode(UNDEFINED_RELAY_PIN, OUTPUT);
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(HEATER_RELAY_PIN, OUTPUT);
  
  
  digitalWrite(SOLENOID_RELAY_PIN, HIGH);
  digitalWrite(UNDEFINED_RELAY_PIN, HIGH);
  digitalWrite(PUMP_RELAY_PIN, HIGH);
  digitalWrite(HEATER_RELAY_PIN, HIGH);
  
  pinMode(RTC_STATUS, OUTPUT);
  pinMode(MS5803_STATUS, OUTPUT);
  pinMode(BNO055_STATUS, OUTPUT);
  pinMode(DOWNLINK_STATUS, OUTPUT);

  pinMode(HASP_DISCRETE_1, INPUT);
  pinMode(HASP_DISCRETE_2, INPUT);
  pinMode(HASP_DISCRETE_3, INPUT);
  pinMode(HASP_DISCRETE_4, INPUT);
  
}



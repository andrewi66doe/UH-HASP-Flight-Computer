
#include <SoftwareSerial.h>
// Comment this line to turn off debug messages
//#define DEBUG


#include "devices.h"
#include "pins.h"
#include "debug.h"
#include "Scheduler.h"
#include "sma.h"
#include "downlink.h"

SoftwareSerial LCD(5,3);
#include "songs.h"
// Functions pertaining to i2c devices
#include "i2c.h"
// Functions pertaining to analog devices
#include "analog.h"

// Callbacks
void read_temp_cb();
void read_pressure_cb();
void read_humidity_cb();
void read_uv_cb();
void read_accel_cb();

// Tasks
Task read_temp(100, TASK_FOREVER, &read_temp_cb);
Task read_pressure(PRESSURE_INTERVAL, TASK_FOREVER, &read_pressure_cb);
Task read_humidity(100, TASK_FOREVER, &read_humidity_cb);
Task read_uv(UV_INTERVAL, TASK_FOREVER, &read_uv_cb);
Task read_accel(500, TASK_FOREVER, &read_accel_cb);

Scheduler scheduler;



void read_temp_cb()
{
  // Take temperature readings from all temp sensors?
  // Just TEMP_0 for now though
  message("Taking temperature readings...");
  send_data(TEMP_0, readTemp(TEMP_0_PIN));
//  send_data(TEMP_1, readTemp(TEMP_1_PIN));
  send_data(TEMP_2, readTemp(TEMP_2_PIN));
//  send_data(TEMP_3, readTemp(TEMP_3_PIN));
//  send_data(TEMP_4, readTemp(TEMP_4_PIN));
//  send_data(TEMP_5, readTemp(TEMP_5_PIN));
  send_data(TEMP_6, readTemp(TEMP_6_PIN));
}

void read_pressure_cb()
{
  // Take a pressure reading
  message("Taking pressure readings...");
  send_data(PRESSURE_S, getPressure());
}

void read_humidity_cb()
{
  message("Taking humidity readings...");
  send_data(HUMIDITY, readHumidityReal(HUMIDITY_PIN));
}

void read_uv_cb()
{
  // Take readings from all UV sensors
  message("Taking UV readings...");
  readUV(PHOTO_0_PIN);
  readUV(PHOTO_1_PIN);
  readUV(PHOTO_2_PIN);
}

void read_gyro_cb()
{
  // Take readings from the gyrocope
  message("Taking gyroscope readings...");
  // Not currently implemented as we're having trouble with the gyro hardware
}

void read_accel_cb() {
  // Take readings from the mpu9250
  message("Taking accelerometer readings...");
  readBNO055();
}

void wait_rpi_initialization()
{
  byte ack_byte;
  
  do{
       wait_for_rx();
       ack_byte = Serial.read();
       if(ack_byte == 0xdd){
        debugInterface();
        break;
      }
      if(ack_byte != 0xff){
        warning("Did not recieve valid ack byte!! Trying again...");
      }
  }while(ack_byte != 0xff);
  
}

void setup() {
  
  Serial.begin(115200);
  // Wait for Rpi to initialize
  wait_rpi_initialization();
  
  downlink.begin(HASP_BAUD_RATE);
  delay(500);
  
  message("Initializing...");
  //PlaySong(imperial_march, 14);
  
  setupPins();

  message("Initializing RTC...");
  setupRTC();
  message("Initialized RTC successfully.");
  delay(500);
  
  message("Initializing Pressure Sensor...");
  setupMS5803();
  message("Initialized MS5803 successfully.");
  delay(500);

  message("Initializing BNO055...");
//  setupBNO055();
  bno.begin();
  digitalWrite(BNO055_STATUS, HIGH);
  message("Initialized BNO055 successfully.");

  // Initialize task scheduler
  scheduler.init();

  // Add tasks to scheduler
  scheduler.addTask(read_temp);
  scheduler.addTask(read_pressure);
  scheduler.addTask(read_humidity);
  scheduler.addTask(read_uv);
  //scheduler.addTask(read_accel);
  
  // Enable tasks
  read_temp.enable();
  read_pressure.enable();
  read_humidity.enable();
  //read_uv.enable();
  //read_accel.enable();

  
  // Turn on pump
  digitalWrite(PUMP_RELAY_PIN, LOW);
  digitalWrite(SOLENOID_RELAY_PIN, LOW);

}


float readTemp2()
{
  return readTemp(TEMP_2_PIN).toFloat();
}

float readTemp0()
{
  return readTemp(TEMP_0_PIN).toFloat();
}

float readPressure()
{
  return pressure_sensor.getPressure(ADC_512);
}

SMAFilter tmp0_filter(5, readTemp0);
SMAFilter tmp2_filter(5, readTemp2);
SMAFilter pressure_filter(5, readPressure);

void loop() {
  readBNO055();
  rtc.update();
  scheduler.execute();
  delay(500);
}






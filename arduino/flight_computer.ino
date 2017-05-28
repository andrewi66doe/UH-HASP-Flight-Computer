
// Comment this line to turn off debug messages
#define DEBUG

#include "debug.h"
#include "devices.h"
#include "pins.h"
#include "Scheduler.h"

// Functions pertaining to i2c devices
#include "i2c.h"
// Functions pertaining to analog devices
#include "analog.h"
// Functions pertaining to digital devices
#include "digital.h"

// Callbacks
void read_temp_cb();
void read_pressure_cb();
void read_humidity_cb();
void read_uv_cb();

// Tasks
Task read_temp(TEMP_INTERVAL, TASK_FOREVER, &read_temp_cb);
Task read_pressure(TEMP_INTERVAL, TASK_FOREVER, &read_pressure_cb);
Task read_humidity(TEMP_INTERVAL, TASK_FOREVER, &read_humidity_cb);
Task read_uv(TEMP_INTERVAL, TASK_FOREVER, &read_uv_cb);

Scheduler scheduler;


void read_temp_cb()
{
  // Take temperature readings from all temp sensors?
  // Just TEMP_0 for now though
  message("Taking temperature readings...");
  send_data(TEMP_0, readTemp(TEMP_0_PIN));
  send_data(TEMP_1, String(pressure_sensor.getTemperature(FAHRENHEIT, ADC_2048)));
  send_data(TEMP_2, readTemp(TEMP_2_PIN));
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
  send_data(HUMIDITY, readHumidity(HUMIDITY_PIN));
}

void read_uv_cb()
{
  // Take readings from all UV sensors
  message("Taking UV readings...");
  readUV(PHOTO_0_PIN);
  //send_data(PHOTO_0, readUV(PHOTO_0_PIN));
  //send_data(PHOTO_1, readUV(PHOTO_1_PIN));
  //send_data(PHOTO_2, readUV(PHOTO_2_PIN));
}

void read_gyro_cb()
{
  // Take readings from the gyrocope
  message("Taking gyroscope readings...");
}

void read_accel_cb() {
  // Take readings from the mpu9250
  message("Taking accelerometer readings...");
  mpu9250_read();
}


void setup() {
  Serial.begin(115200);
  analogReference(EXTERNAL);
  
  message("Initializing...");

  message("Initializing RTC...");
  rtc.begin(DS13074_CS_PIN);
  rtc.autoTime();
  rtc.set12Hour();
  message("Initialized RTC successfully.");

  message("Initializing Pressure Sensor...");
  pressure_sensor.reset();
  pressure_sensor.begin();
  message("Initialized Pressure Sensor successfully.");

  message("Initializing MPU9250...");
  mpu9250_setup();
  message("Initialized MPU9250 successfully.");
 
  scheduler.init();
  scheduler.addTask(read_temp);
  scheduler.addTask(read_pressure);
  scheduler.addTask(read_humidity);
  scheduler.addTask(read_uv);
}

void loop() {
  rtc.update();
  scheduler.execute();
}






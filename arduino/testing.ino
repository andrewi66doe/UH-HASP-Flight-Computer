#define DEBUG
#include "debug.h"
#include "devices.h"
#include "pins.h"
#include "Scheduler.h"

// Functions pertaining to analog devices
#include "analog.h"
// Functions pertaining to i2c devices
#include "i2c.h"
// Functions pertaining to digital devices
#include "digital.h"



Scheduler scheduler = Scheduler();

void setup() {
  Serial.begin(115200);
  // Set up pressure sensor
  pressure_sensor.reset();
  pressure_sensor.begin();
  
  // For now take readings on all sensors once every two seconds
  // We can change this in the future, and sensors can be scheduled independently
  scheduler.schedule(read_temp, 2000);
  scheduler.schedule(read_pressure, 2000);
  scheduler.schedule(read_humidity, 2000);
  scheduler.schedule(read_uv, 2000);
  scheduler.schedule(read_gyro, 2000);
}

void loop() {
  scheduler.update();
}

void read_temp()
{
  // Take temperature readings from all temp sensors?
  // Just TEMP_0 for now though
  message("Taking temperature readings...");
  send_data(TEMP_0, readTemp(TEMP_0_PIN));
  scheduler.schedule(read_temp, 2000);
}

void read_pressure()
{
  // Take a pressure reading
  message("Taking pressure readings...");
  send_data(PRESSURE_S, getPressure());
  scheduler.schedule(read_pressure, 2000);
}

void read_humidity()
{
  message("Taking humidity readings...");
  send_data(HUMIDITY, readHumidity(HUMIDITY_PIN));
  scheduler.schedule(read_humidity, 2000);
}

void read_uv()
{
  // Take readings from all UV sensors
  message("Taking UV readings...");
  scheduler.schedule(read_uv, 2000);
}

void read_gyro()
{
  // Take readings from the gyrocope
  message("Taking gyroscope readings...");
  scheduler.schedule(read_gyro, 2000);
}


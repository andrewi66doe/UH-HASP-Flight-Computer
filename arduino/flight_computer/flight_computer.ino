
#include <SoftwareSerial.h>
#include <TinyGPS.h>

// Comment this line to turn off debug messages
//#define DEBUG


void displaySensorStatus();

#include "devices.h"
#include "pins.h"

#include "sma.h"

// Functions pertaining to i2c devices
#include "i2c.h"
// Functions pertaining to analog devices
#include "analog.h"
#include "control.h"
#include "debug.h"
#include "downlink.h"
#include "tasks.h"




void setup() {

  // RPI serial interface
  Serial.begin(115200);
  
  setupPins();

  // Downlink serial interface
  Serial1.begin(4800);
  delay(500);

  // Initialize GPS serial interface
  Serial2.begin(57600);
  delay(500);
  
  message("Initializing...");  
  message("Initializing RTC...");
  setupRTC();
  message("Initialized RTC successfully.");
  delay(500);
  
  message("Initializing Pressure Sensor...");
  setupMS5803();
  message("Initialized MS5803 successfully.");
  delay(500);

  message("Initializing BNO055...");
  setupBNO055();
//  bno.begin();
  digitalWrite(BNO055_STATUS, HIGH);
  message("Initialized BNO055 successfully.");

  initialize_tasks();
  
  // Enable tasks
  read_temp.enable();
  read_pressure.enable();
  read_humidity.enable();
  read_uv.enable();
  read_gps.enable();
  downlink_packet.enable();
}

void loop() {
  rtc.update();
  readBNO055();
  scheduler.execute();
  printOrientation();
  send_data(HEADING, String(CompassHeading()));
  
  hasp_discrete_passthrough();
  
  if(Serial.available() > 0){
    char in = Serial.read();
    if(in == 'd'){
      debugInterface();
    }
  }
  delay(50);
  smartdelay(100);
}



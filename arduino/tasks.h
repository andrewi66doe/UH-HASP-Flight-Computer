#include "Scheduler.h"


Scheduler scheduler;

// Callbacks
void read_temp_cb();
void read_pressure_cb();
void read_humidity_cb();
void read_uv_cb();
void read_accel_cb();
void read_gps_cb();
void send_packet_cb();

float readTemp0();
float readTemp1();
float readTemp2();
float readTemp3();
float readTemp6();

float readUV0();
float readUV1();
float readUV2();


float readPressure();

// Tasks
Task read_temp(TEMP_INTERVAL, TASK_FOREVER, &read_temp_cb);
Task read_pressure(PRESSURE_INTERVAL, TASK_FOREVER, &read_pressure_cb);
Task read_humidity(HUMIDITY_INTERVAL, TASK_FOREVER, &read_humidity_cb);
Task read_uv(UV_INTERVAL, TASK_FOREVER, &read_uv_cb);
Task read_gps(GPS_INTERVAL, TASK_FOREVER, &read_gps_cb);
Task downlink_packet(3000, TASK_FOREVER, &send_packet_cb);

SMAFilter tmp0_filter(5, readTemp0);
SMAFilter tmp1_filter(5, readTemp1);
SMAFilter tmp2_filter(5, readTemp2);
SMAFilter tmp3_filter(5, readTemp3);
SMAFilter tmp6_filter(5, readTemp6);
SMAFilter pressure_filter(5, readPressure);
SMAFilter uv0_filter(5, readUV0);
SMAFilter uv1_filter(5, readUV1);
SMAFilter uv2_filter(5, readUV2);

TinyGPS gps;

float readTemp2()
{
  return readTemp(TEMP_2_PIN).toFloat();
}

float readTemp0()
{
  return readTemp(TEMP_0_PIN).toFloat();
}

float readTemp1()
{
  return readTemp(TEMP_1_PIN).toFloat();
}
float readTemp3()
{
  return readTemp(TEMP_3_PIN).toFloat();  
}

float readTemp6()
{
  return readTemp(TEMP_6_PIN).toFloat();
}

float readPressure()
{
  return pressure_sensor.getPressure(ADC_512);
}

float readUV0()
{
  return readUV(PHOTO_0_PIN);
}

float readUV1()
{
  return readUV(PHOTO_1_PIN);
}

float readUV2()
{
  return readUV(PHOTO_0_PIN);
}



void read_temp_cb()
{
  // Take temperature readings from all temp sensors?
  // Just TEMP_0 for now though
  message("Taking temperature readings...");
  tmp_0 = tmp0_filter.getFilteredSample();
  send_data(TEMP_0, String(tmp_0));
  delay(10);
  
  tmp_1 = tmp1_filter.getFilteredSample();
  send_data(TEMP_1, String(tmp_1));
  delay(10);

  tmp_2 = tmp2_filter.getFilteredSample();
  send_data(TEMP_2, String(tmp_2));
  delay(10);

  tmp_3 = tmp3_filter.getFilteredSample();
  send_data(TEMP_3, String(tmp_3));
  delay(10);
//  send_data(TEMP_4, readTemp(TEMP_4_PIN));
//  send_data(TEMP_5, readTemp(TEMP_5_PIN));
  tmp_6 = tmp6_filter.getFilteredSample();
  send_data(TEMP_6, String(tmp_6));
  delay(10);
}

void read_pressure_cb()
{
  // Take a pressure reading
  message("Taking pressure readings...");
  pressure = pressure_filter.getFilteredSample();
  send_data(PRESSURE_S, String(pressure));
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
  
  send_data(PHOTO_0, String(uv0_filter.getFilteredSample()));
  send_data(PHOTO_1, String(uv1_filter.getFilteredSample()));
  send_data(PHOTO_2, String(uv2_filter.getFilteredSample()));
}

void send_packet_cb()
{
  digitalWrite(DOWNLINK_STATUS, HIGH);
  send_data_packet();
  digitalWrite(DOWNLINK_STATUS, LOW);
}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial2.available())
      gps.encode(Serial2.read());
  } while (millis() - start < ms);
}


void read_gps_cb()
{
  float flat, flon;
  unsigned long age;
  gps.f_get_position(&flat, &flon, &age);
  
  // If we don't have a valid GPS fix don't print
  if(flat == TinyGPS::GPS_INVALID_F_ANGLE)
    return;
    
  send_data(SAT_COUNT, String(gps.satellites()));
  send_data(GEOPOSITION, String(flat) + "-" + String(flon));
  send_data(ALTITUDE, String(gps.f_altitude()));
}


void initialize_tasks()
{
  // Initialize task scheduler
  scheduler.init();

  // Add tasks to scheduler
  scheduler.addTask(read_temp);
  scheduler.addTask(read_pressure);
  scheduler.addTask(read_humidity);
  scheduler.addTask(read_uv);
  scheduler.addTask(read_gps);
  scheduler.addTask(downlink_packet);
}



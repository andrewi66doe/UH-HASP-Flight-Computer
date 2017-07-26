#define HASP_SERIAL_RX_PIN 41
#define HASP_SERIAL_TX_PIN 42

#define HASP_BAUD_RATE 4800

float tmp_0,tmp_1,tmp_2,tmp_3,tmp_6;
float pressure;

int packet_id = 0;

typedef struct data_packet_t {
  float pressure;
  float temperature[8];
  float humidity;
  float gyroscope[3];
  float accelerometer[3];
  float magnetometer[3];
}data_packet_t;


void populate_packet(data_packet_t *packet);

void send_data_packet()
{
  data_packet_t packet;  
  char buf[sizeof(data_packet_t)];
  
  populate_packet(&packet);
  
  memcpy(buf, &packet, sizeof(packet));
  
  Serial1.write(packet_id);
  
  int sent = Serial1.write(buf, sizeof(data_packet_t));
  
  if(sent < sizeof(data_packet_t)){
    Serial.print("Sent " + String(sent) + " of "  + String(sizeof(data_packet_t)) + " bytes");
  }
  Serial1.write(rtc.getDay());
  Serial1.write(rtc.getMinute());
  Serial1.write(rtc.getSecond());
  packet_id++;  
}

void populate_packet(data_packet_t* packet)
{
  // Zero out struct
  memset(packet, 0, sizeof(data_packet_t));

  packet->temperature[0] = tmp_0;
  packet->temperature[1] = tmp_1;
  packet->temperature[2] = tmp_2;
  packet->temperature[3] = tmp_3;
  packet->temperature[6] = tmp_6;

  packet->gyroscope[0] = gyro_x;
  packet->gyroscope[1] = gyro_y;
  packet->gyroscope[2] = gyro_z;

  packet->accelerometer[0] = accel_x;
  packet->accelerometer[1] = accel_y;
  packet->accelerometer[2] = accel_z;

  packet->magnetometer[0] = mag_x;
  packet->magnetometer[1] = mag_y;
  packet->magnetometer[2] = mag_z;
  packet->pressure = pressure;
  
  
}


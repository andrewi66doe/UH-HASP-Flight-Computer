
void message(String msg)
{
  #ifdef DEBUG
  Serial.println("[MSG] " + msg);
  #endif
}

void warning(String msg)
{
  #ifdef DEBUG
  Serial.println("[WARNING] " + msg);
  #endif
}

void error(String msg)
{
  #ifdef DEBUG
  Serial.println("[ERROR] " + msg);
  #endif
}


void printBits(uint16_t myByte){
 for(uint16_t mask = 0x8000; mask; mask >>= 1){
   if(mask  & myByte)
       Serial.print('1');
   else
       Serial.print('0');
 }
}


void wait_for_rx()
{
  while(Serial.available() < 1){
     delay(10);
  }
}

int debugInterface()
{
  // Wait to recieve a command in rx buffwe
  wait_for_rx();
  
  byte cmd = Serial.read();
  
  while(cmd != 'q'){
    // Wait for command in rx buffer
    wait_for_rx();
    
    byte cmd = Serial.read();

    bool pump_on = false;
    bool heater_on = false;
    bool solenoid_on = false;
    
    switch(cmd){
      case 'p':
        // Toggle pump
        digitalWrite(PUMP_RELAY_PIN, LOW);
        pump_on = !pump_on;
        break;
      case 'h':
        // Toggle heater
        digitalWrite(HEATER_RELAY_PIN, LOW);
        heater_on = !heater_on;
        break;
      case 's':
        // Toggle solenoid
        digitalWrite(SOLENOID_RELAY_PIN, LOW);
        solenoid_on = !solenoid_on;
        break;
      case 'i':
        setupPins();
        break;
      default:
        Serial.println("Invalid command received!");
        break;
    }

  }
}


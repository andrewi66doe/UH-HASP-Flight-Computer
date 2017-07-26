

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

void printHelp()
{
  Serial.println("UH-HASP Debug Interface V 1.0");
  Serial.println();
  Serial.println("Available commands:");
  Serial.println();
  Serial.println("p | s | h   Toggle (p)ump (s)olenoid (h)eater");
  Serial.println("a           Display BNO055 status");
  Serial.println("c           Display BNO055 orientation/calibration");
  Serial.println("r           Print readings from all connected sensors");
  Serial.println("q           Exit the debug interface");
  Serial.println("i           Display this message");
  
}

int debugInterface()
{
  Serial.println("UH-HASP Debug Interface V 1.0");
  Serial.println("Welcome to the debug interface!");
  Serial.print(">");
    
  char cmd = 'n';
  
  while(cmd != 'q'){
    if(Serial.available() > 0){
      cmd = Serial.read();
      Serial.println(cmd);
      
      switch(cmd){
        case 'p':
          // Toggle pump
          if(!pump_on){
            digitalWrite(PUMP_RELAY_PIN, LOW);
            Serial.println("Pump turned on");
          }else{
            digitalWrite(PUMP_RELAY_PIN, HIGH);
            Serial.println("Pump turned off");
          }
          pump_on = !pump_on;
          break;
        case 'h':
          // Toggle heater
          if(!heater_on){
            digitalWrite(HEATER_RELAY_PIN, LOW);
            Serial.println("Heater turned on");
          }else{
            digitalWrite(HEATER_RELAY_PIN, HIGH);
            Serial.println("Heater turned off");
          }
          
          heater_on = !heater_on;
          break;
        case 's':
          // Toggle solenoid
          if(!solenoid_on){
            digitalWrite(SOLENOID_RELAY_PIN, LOW);
            Serial.println("Solenoid turned on");
          }else{
            digitalWrite(SOLENOID_RELAY_PIN, HIGH);
            Serial.println("Solenoid turned off");
          }
          solenoid_on = !solenoid_on;
          break;
        case 'u':
          // Toggle solenoid
          if(!undefined_on){
            digitalWrite(UNDEFINED_RELAY_PIN, LOW);
            Serial.println("Undefined turned on");
          }else{
            digitalWrite(UNDEFINED_RELAY_PIN, HIGH);
            Serial.println("Undefined turned off");
          }
          undefined_on = !undefined_on;
          break;
        case 'a':
          Serial.println("Displaying BNO055 status");
          displaySensorStatus();
          break;
        case 'c':
          Serial.println("Displaying BNO055 orientation/calibration");
          printOrientation();
          break;
        case 'r':
          Serial.println("Displaying sensor readings");
          //scheduler.execute();
          break;
        case 'i':
          printHelp();
          break;
        default:
          Serial.println("Invalid command received!!");
      }
      Serial.print(">");
  
    }
  }
}


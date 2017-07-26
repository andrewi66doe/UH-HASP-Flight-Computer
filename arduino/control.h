

bool pump_on = false;
bool solenoid_on = false;
bool heater_on = false;
bool undefined_on = false;


int disc_1=0, disc_2=0, disc_3=0, disc_4=0;


void turn_pump_on()
{
  digitalWrite(PUMP_RELAY_PIN, LOW);
  pump_on = true;
}

void turn_solenoid_on()
{
  digitalWrite(SOLENOID_RELAY_PIN, LOW);
  solenoid_on = true;
}

void turn_heater_on()
{
  digitalWrite(HEATER_RELAY_PIN, LOW);
  heater_on = true;
}


void turn_pump_off()
{
  digitalWrite(PUMP_RELAY_PIN, HIGH);
  pump_on = false;
}

void turn_solenoid_off()
{
  digitalWrite(SOLENOID_RELAY_PIN, HIGH);
  solenoid_on = false;
}

void turn_heater_off()
{
  digitalWrite(HEATER_RELAY_PIN, HIGH);
  heater_on = false;
}

void hasp_discrete_passthrough()
{
  disc_1 = digitalRead(HASP_DISCRETE_1);
  disc_2 = digitalRead(HASP_DISCRETE_2);
  disc_3 = digitalRead(HASP_DISCRETE_3);
  disc_4 = digitalRead(HASP_DISCRETE_4);

  if(disc_1 == HIGH){
    turn_pump_on();
  }else{
    turn_pump_off();
  }
  if(disc_2 == HIGH){
    turn_solenoid_on();
  }else{
    turn_solenoid_off();
  }
  if(disc_3 == HIGH){
    turn_heater_on();
  }else{
    turn_heater_off();
  }
  if(disc_3 == HIGH){
    // Undefined at the moment
  }else{
    // Also undefined
  }
  
}



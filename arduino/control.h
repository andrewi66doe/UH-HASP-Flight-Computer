

bool pump_on = false;
bool solenoid_on = false;
bool heater_on = false;
bool undefined_on = false;

Servo servo;
int disc_1=0, disc_2=0, disc_3=0, disc_4=0;


void turn_pump_on()
{
  digitalWrite(PUMP_RELAY_PIN, LOW);
  pump_on = true;
}

void turn_solenoid_on()
{
  // Full duty cycle 25 volts 
  analogWrite(SOLENOID_PWM, 200);
  // Wait for solenoid to engage
  delay(500);
  // Lower duty cycle to 9 volts to keep solenoid open
  analogWrite(SOLENOID_PWM, 77);
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
  analogWrite(SOLENOID_PWM, 0);
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

  if(disc_1 == LOW){
    if(!solenoid_on)
      turn_solenoid_on();
    delay(500);
    turn_pump_on();
  }
  if(disc_2 == LOW){
    if(solenoid_on)
      turn_solenoid_off();
    delay(500);
    turn_pump_off();
    turn_heater_off();
  }
  if(disc_3 == LOW){
    turn_heater_on();
    delay(100);
  }

}

void servo_sweep(int start_pos, int end_pos) {
  delay(15);

  servo.write(15);
}

void servo_reset() {

    servo.write(0);

}




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


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

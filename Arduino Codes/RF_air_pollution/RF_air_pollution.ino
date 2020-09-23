#include <VirtualWire.h> //Download it here: http://electronoobs.com/eng_arduino_virtualWire.php
#define size 1
int pot = A0;
byte TX_buffer[size]={0};
byte i;


void setup() 
{
  // virtual wire
  vw_set_tx_pin(3); // pin
  vw_setup(2000); // bps
  for(i=0;i<size;i++)
  {
     TX_buffer[i]=i;
  }
}

void loop()
{ 
  int val = map(analogRead(pot),0,1024,0,255);
  TX_buffer[0] = 250;  
  vw_send(TX_buffer, size); 
  vw_wait_tx(); 
  delay(10);   
}

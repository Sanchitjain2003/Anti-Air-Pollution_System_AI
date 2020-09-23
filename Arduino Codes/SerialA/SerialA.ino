#include <SoftwareSerial.h>
SoftwareSerial s(D5,D6);
 
void setup() {
s.begin(9600);
}
 
void loop() {
int data=150;
if(s.available()>0)
{
 s.write(data);
}
}

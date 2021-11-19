#include "SensorsSDP.h"

bool SonarSensor(int trigPinSensor, int echoPinSensor, int LED, int threshold)
{
//generate the ultrasonic wave
digitalWrite(trigPinSensor, LOW); //put trigpin low
delayMicroseconds(2); //wait 2 microseconds
digitalWrite(trigPinSensor, HIGH); // switch trigpin high
delayMicroseconds(10); //wait 10 microseconds 
digitalWrite(trigPinSensor, LOW); // turn low again

//read the distance
long duration = pulseIn(echoPinSensor, HIGH);
long distance = (duration/2)/29.1;

//LEDs for sensor 1
if(distance <= threshold) //if distance is less than 10 cm 
{
  digitalWrite(LED, HIGH);
  return true;
}
else
{
  digitalWrite(LED, LOW);
  return false;
}
}

#include "SensorsSDP.h"
#define trigPin 9
#define trigPin2 13
#define trigPin3 7
#define trigPin4 11

#define echoPin 8
#define echoPin2 12
#define echoPin3 6
#define echoPin4 10

#define led A4 //red
#define led2 A0 //red
#define led3 A3 //white
#define led4 A1 //white

//used varibles 
long duration, distance, UltraSensor, UltraSensor2,UltraSensor3, UltraSensor4;
char data;
String SerialData;

void setup()
{ 
Serial.begin (9600);
//sensor 1
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(led, OUTPUT);
//sensor 2
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
pinMode(led2, OUTPUT);
//sensor 3
pinMode(trigPin3, OUTPUT);
pinMode(echoPin3, INPUT);
pinMode(led3, OUTPUT);
//sensor 4
pinMode(trigPin4, OUTPUT);
pinMode(echoPin4, INPUT);
pinMode(led4, OUTPUT);
//initialize LED status
digitalWrite(led,LOW);
digitalWrite(led2,LOW);
digitalWrite(led3,LOW);
digitalWrite(led4,LOW);
}

void loop()
{ 
//store distance for sesnor 1 
SonarSensor(trigPin,echoPin, led, 100);
//store distance for sesnor 2
SonarSensor(trigPin2,echoPin2,led2, 100);
//store distance for sesnor 3
SonarSensor(trigPin3,echoPin3,led3, 100); 
//store distance for sesnor 4
SonarSensor(trigPin4,echoPin4,led4, 100);

SerialData="";
}


#include "motor_control.h"

#define AIN1 27
#define BIN1 32
#define AIN2 33
#define BIN2 14
#define PWMA 15
#define PWMB 13
#define STBY 21
#define FORWARD 1
#define BACKWARD 0
#define STOP 0
#define BEGIN 1
#define ULTRA_DIRECTION 26
#define ULTRA_DETECTION 36
#define ULTRA_SLEEP 25


void setup(){
  motorSetup(PWMA, PWMB, AIN1, AIN2, BIN1, BIN2, STBY);
  pinMode(ULTRA_DIRECTION, OUTPUT);
  pinMode(ULTRA_DETECTION, INPUT);
  pinMode(ULTRA_SLEEP, OUTPUT);
}


void loop(){
  forwardRight(255, 2);
  delay(5000);
  forwardRight(255, 4);
  delay(5000);
  forwardRight(255, 8);
  delay(5000);
  forwardRight(255, 16);
  delay(5000);
  }

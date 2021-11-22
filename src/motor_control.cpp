#include "motor_control.h"

uint8_t PWMA;
uint8_t PWMB;
uint8_t AIN1;
uint8_t AIN2;
uint8_t BIN1;
uint8_t BIN2;
uint8_t STBY;
uint8_t ULTRA_DIRECTION;
uint8_t ULTRA_DETECTION;
uint8_t ULTRA_SLEEP;


void motorSetup(uint8_t pwmA, uint8_t pwmB, uint8_t aIn1, uint8_t aIn2, uint8_t bIn1, uint8_t bIn2, uint8_t stby){
  PWMA = pwmA;
  PWMB = pwmB;
  AIN1 = aIn1;
  AIN2 = aIn2;
  BIN1 = bIn1;
  BIN2 = bIn2;
  STBY = stby;
  
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);
  digitalWrite(STBY, HIGH);
}

void ultraSetup(uint8_t directionPin, uint8_t detectionPin, uint8_t sleepPin)
{
  ULTRA_DIRECTION = directionPin;
  ULTRA_DETECTION = detectionPin;
  ULTRA_SLEEP = sleepPin;

  pinMode(ULTRA_DIRECTION, OUTPUT);
  pinMode(ULTRA_DETECTION, INPUT);
  pinMode(ULTRA_SLEEP, OUTPUT);
}

void forward(int speed){
  digitalWrite(ULTRA_DIRECTION, HIGH);
  analogWrite(PWMA, speed);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMB, speed);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void backward(int speed){
  digitalWrite(ULTRA_DIRECTION, LOW);
  analogWrite(PWMA, speed);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMB, speed);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void forwardLeft(int speed, int size){
  digitalWrite(ULTRA_DIRECTION, HIGH);
  analogWrite(PWMA, (speed/size));
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMB, speed);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void forwardRight(int speed, int size){
  digitalWrite(ULTRA_DIRECTION, HIGH);
  analogWrite(PWMA, speed);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMB, (speed/size));
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  
}


void backwardLeft(int speed, int size){
  digitalWrite(ULTRA_DIRECTION, LOW);
  analogWrite(PWMA, (speed/size));
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMB, speed);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void backwardRight(int speed, int size){
  digitalWrite(ULTRA_DIRECTION, LOW);
  analogWrite(PWMA, speed);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMB, (speed/size));
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void rotateCCW(int speed){
  analogWrite(PWMA, speed);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMB, speed);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void rotateCW(int speed){
  analogWrite(PWMA, speed);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMB, speed);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void fullStop(){
  analogWrite(PWMA, 0);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMB, 0);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  
}

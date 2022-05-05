#include "motor_control.h"

uint8_t pwmA;
uint8_t pwmB;
uint8_t ain1;
uint8_t ain2;
uint8_t bin1;
uint8_t bin2;
uint8_t stby;
uint8_t ultra_direction;
uint8_t ultra_detection;
uint8_t ultra_sleep;


void motorSetup(uint8_t PWMA, uint8_t PWMB, uint8_t AIN1, uint8_t AIN2, uint8_t BIN1, uint8_t BIN2, uint8_t STBY){
  pwmA = PWMA;
  pwmB = PWMB;
  ain1 = AIN1;
  ain2 = AIN2;
  bin1 = BIN1;
  bin2 = BIN2;
  stby = STBY;
  
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  pinMode(bin1, OUTPUT);
  pinMode(bin2, OUTPUT);
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
  pinMode(stby, OUTPUT);
  digitalWrite(stby, HIGH);
}

void ultraSetup(uint8_t ULTRA_DIRECTION, uint8_t ULTRA_DETECTION, uint8_t ULTRA_SLEEP)
{
  ultra_direction = ULTRA_DIRECTION;
  ultra_detection = ULTRA_DETECTION;
  ultra_sleep = ULTRA_SLEEP;

  pinMode(ultra_direction, OUTPUT);
  pinMode(ultra_detection, INPUT);
  pinMode(ultra_sleep, OUTPUT);
}

void forward(int speed){
  analogWrite(pwmA, speed);
  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);
  analogWrite(pwmB, speed);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);
}

void forwardLeft(int speed, int size){
  analogWrite(pwmA, (speed/size));
  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);
  analogWrite(pwmB, speed);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);
}

void forwardRight(int speed, int size){
  analogWrite(pwmA, speed);
  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);
  analogWrite(pwmB, (speed/size));
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH); 
}

void backward(int speed){
  analogWrite(pwmA, speed);
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, HIGH);
  analogWrite(pwmB, speed);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);
}


void backwardLeft(int speed, int size){
  analogWrite(pwmA, (speed/size));
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, HIGH);
  analogWrite(pwmB, speed);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);
}

void backwardRight(int speed, int size){
  analogWrite(pwmA, speed);
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, HIGH);
  analogWrite(pwmB, (speed/size));
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);
}

void rotateCCW(int speed){
  analogWrite(pwmA, speed);
  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);
  analogWrite(pwmB, speed);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);
}

void rotateCW(int speed){
  analogWrite(pwmA, speed);
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, HIGH);
  analogWrite(pwmB, speed);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);
}

void fullStop(){
  analogWrite(pwmA, 0);
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, LOW);
  analogWrite(pwmB, 0);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, LOW);
  
}

void pivotCW(int speed) {
  analogWrite(pwmA, speed);
  digitalWrite(ain1, HIGH);
  digitalWrite(ain2, LOW);
  analogWrite(pwmB, 0);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, LOW);
}

void pivotCCW(int speed) {
  analogWrite(pwmA, 0);
  digitalWrite(ain1, LOW);
  digitalWrite(ain2, LOW);
  analogWrite(pwmB, speed);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);
}

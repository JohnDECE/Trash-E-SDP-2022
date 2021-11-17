
#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H
#include <analogWrite.h>

void motorSetup(uint8_t pwmA, uint8_t pwmB, uint8_t aIn1, uint8_t aIn2, uint8_t bIn1, uint8_t bIn2, uint8_t stby);

void forward(int speed);

void backward(int speed);

void forwardLeft(int speed, int size);

void forwardRight(int speed, int size);

void backwardLeft(int speed, int size);

void backwardRight(int speed, int size);

void rotateCW(int speed);

void rotateCCW(int speed);

void fullStop();

void ninety_left(int speed);


 
#endif

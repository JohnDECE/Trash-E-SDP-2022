#ifndef PHOTO_CELL_CALIBRATOR_H
#define PHOTO_CELL_CALIBRATOR_H

#include "Photo_Cell_Calibrator.h"
#include <arduino.h>

void fullCalibration(uint8_t leftPhotoresistor, uint8_t rightPhotoresistor, int *leftMax, int *leftMin, int *rightMax, int *rightMin, int calibrationTime = 5000);
int readPhotoCells(uint8_t LeftPhotoresistor, uint8_t RightPhotoresistor, int *leftMin, int *leftMax, int *rightMin, int *rightMax, bool reverseColor);

#endif

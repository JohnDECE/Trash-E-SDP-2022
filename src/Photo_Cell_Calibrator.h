#ifndef PHOTO_CELL_CALIBRATOR_H
#define PHOTO_CELL_CALIBRATOR_H

#include "Photo_Cell_Calibrator.h"
#include <arduino.h>
#include "motor_control.h"

#define LeftLEDPin 25
#define RightLEDPin 26

struct photoVals {
  int Left_Photo;
  int Right_Photo;
};

void fullCalibration(uint8_t leftPhotoresistor, uint8_t rightPhotoresistor, int analogResolution, int calibrationTime = 5000);
void trackLine(uint8_t LeftPhotoresistor, uint8_t RightPhotoresistor, bool reverseColor, photoVals *photoValues);

#endif

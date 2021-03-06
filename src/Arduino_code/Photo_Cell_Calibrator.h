#ifndef PHOTO_CELL_CALIBRATOR_H
#define PHOTO_CELL_CALIBRATOR_H

#include "Photo_Cell_Calibrator.h"
#include <Arduino.h>

//#define LeftLEDPin 12
//#define RightLEDPin 4

struct photoVals {
  int Left_Photo;
  int Right_Photo;
};

void setupPhotoresistors(uint8_t leftLED, uint8_t rightLED, uint8_t leftPhoto, uint8_t rightPhoto, int analogResolution);
int readRawPhotoVal(uint8_t photoCellPin);
void fullCalibration(uint8_t leftPhotoresistor, uint8_t rightPhotoresistor, int analogResolution, int calibrationTime = 5000);
void trackLine(uint8_t LeftPhotoresistor, uint8_t RightPhotoresistor, bool reverseColor, photoVals *photoValues, bool towards_pickup, uint8_t motorSpeed);

#endif

#include "Photo_Cell_Calibrator.h"

uint8_t RightPhotoResistor = 34;
uint8_t LeftPhotoResistor = 39;
//uint8_t LeftLEDPin = 25;
//uint8_t RightLEDPin = 26;

int analogResolution = 10;
photoVals photoValues;
// uint8_t totalSamples = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(LeftLEDPin, OUTPUT);
  pinMode(RightLEDPin, OUTPUT);

  // Turn on both LEDs to signify calibration
  digitalWrite(LeftLEDPin, HIGH);
  digitalWrite(RightLEDPin, HIGH);

  fullCalibration(LeftPhotoResistor, RightPhotoResistor, analogResolution);
  
  // Calibration Done.
  digitalWrite(LeftLEDPin, LOW);
  digitalWrite(RightLEDPin, LOW);
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("SETUP DONE.");
}

void loop() {
  // put your main code here, to run repeatedly:
  trackLine(LeftPhotoResistor, RightPhotoResistor, false, &photoValues);
}

#include <Photo_Cell_Calibrator.h>

uint8_t RightPhotoResistor = 34;
uint8_t LeftPhotoResistor = 39;
uint8_t LeftLEDPin = 25;
uint8_t RightLEDPin = 26;

int LeftMax = 0;
int LeftMin = 1023;

int RightMax = 0;
int RightMin = 1023; // We have a 10-bit analog resolution

// uint8_t totalSamples = 3;

void setup() {
  // put your setup code here, to run once:
  pinMode(LeftLEDPin, OUTPUT);
  pinMode(RightLEDPin, OUTPUT);

  // Turn on both LEDs to signify calibration
  digitalWrite(LeftLEDPin, HIGH);
  digitalWrite(RightLEDPin, HIGH);

  fullCalibration(LeftPhotoResistor, RightPhotoResistor, &LeftMax, &LeftMin, &RightMax, &RightMin);
  
  // Calibration Done.
  digitalWrite(LeftLEDPin, LOW);
  digitalWrite(RightLEDPin, LOW);
  
  Serial.begin(115200);
  delay(1000);
  Serial.println("SETUP DONE.");
}

void loop() {
  // put your main code here, to run repeatedly:
  switch (readPhotoCells(LeftPhotoResistor, RightPhotoResistor, &LeftMin, &LeftMax, &RightMin, &RightMax, true)) { //Read photocells with reverse coloring set to true
    case 1: // Left side triggered => left side off line
      digitalWrite(RightLEDPin, HIGH);
      break;
    case 2: // Right side triggered => right side off line
      digitalWrite(LeftLEDPin, HIGH);
      break;
    case -1: // Error occurred, this will occur when 
      break;
    default: // Both sides triggered => off line
      digitalWrite(RightLEDPin, LOW);
      digitalWrite(LeftLEDPin, LOW);
      break;
  }

}

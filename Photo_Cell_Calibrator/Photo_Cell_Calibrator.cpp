#include "Photo_Cell_Calibrator.h"

bool calibrated = false;

void CalibratePhotoresistors(uint8_t sensePin, int *senseMax, int *senseMin) {
  /*
   * This function will perform one iteration of a photo cell calibration.
   * 
   * sensePin: A pointer to a value that represents the input pin reading our photo cell.
   * senseMax: A pointer to a value that represents the maximum value read by our photo cell, this will get calibrated.
   * senseMin: A pointer to a value that represents the minimum value read by our photo cell, this will get calibrated.
   */
  int sensorVal = analogRead(sensePin);

  if (sensorVal > *senseMax) {
    *senseMax = sensorVal;
  }

  if (sensorVal < *senseMin) {
    *senseMin = sensorVal;
  }
}

void fullCalibration(uint8_t leftPhotoresistor, uint8_t rightPhotoresistor, int *leftMax, int *leftMin, int *rightMax, int *rightMin, int calibrationTime) {
  /*
   * Calibrate both photo cells then return true to indicate success.
   * 
   * calibrationTime: Defaults to 5000 ms, simply sets the total time (in ms) we calibrate for.
   */
  analogReadResolution(10);
  while (millis() < calibrationTime) {
    CalibratePhotoresistors(leftPhotoresistor, leftMax, leftMin);
    CalibratePhotoresistors(rightPhotoresistor, rightMax, rightMin);
  }
  calibrated = true;
}

int readPhotoCells(uint8_t LeftPhotoresistor, uint8_t RightPhotoresistor, int *leftMin, int *leftMax, int *rightMin, int *rightMax, bool reverseColor) {
  /*
   * Should run after sensors are calibrated
   * If not calibrated then do nothing.
   * 
   * This function will read our photocells at pins LeftPhotoresistor and RightPhotoresistor. 
   * Then using our calibrated values, we will force the read values in between our calibrated values and then convert our squeezed values into a boolean like value.
   * Using that value, we will then determine if we are off or on our line. The line color is a light or dark color which is decided by the reverseColor boolean.
   * A true value indicates a light colored line and vice versa.
   * 
   * reverseColor: False means that we are doing a dark colored line and a bright colored outside, true is the opposite
   */
  if (calibrated) {
    int Left_Photo = analogRead(LeftPhotoresistor);
    int Right_Photo = analogRead(RightPhotoresistor);
    
    Left_Photo = constrain(Left_Photo, *leftMin, *leftMax);
    Right_Photo = constrain(Right_Photo, *rightMin, *rightMax);
  
    Left_Photo = map(Left_Photo, *leftMin, *leftMax, 0, 2); // Anything greater than 0 indicates a light surface, anything that is 0 indicates a dark surface
    Right_Photo = map(Right_Photo, *rightMin, *rightMax, 0, 2); 
    
    if (reverseColor) { // Light is the line, dark is everything else
      if ((Left_Photo && !Right_Photo) || (!Left_Photo && Right_Photo)) { // If we detect dark, it must exclusively be detected on one side (This is the XOR of Left_Photo and Right_Photo)
        if (!Left_Photo) { // Left Side is dark (like asphalt)
          return 1;
        }
        else { // Right Side is dark (like asphalt)
          return 2;
        }
      }
      else if (Left_Photo && Right_Photo) { // Since we reversed the colors, we know we are on the line if we detect white on both sides, thus turn off the LEDs
        return 0;
      }
    }
    else { //Dark line, light outside
      if ((Left_Photo && !Right_Photo) || (!Left_Photo && Right_Photo)) { // If we detect dark, it must exclusively be detected on one side
        if (Left_Photo) { // Left Side is light (Like if we are using dark colored tape on a bright surface)
          return 1;
        }
        else { //Right Side is light (Like if we are using dark colored tape on a bright surface)
          return 2;
        }
      }
      else if (!Left_Photo && !Right_Photo) { // If both sensors detect dark then turn off the LEDs
        return 0;
      }
    }
  }
  return -1;
}

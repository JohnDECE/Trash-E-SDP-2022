#include "Photo_Cell_Calibrator.h"

bool calibrated = false;

int LeftMax = 0;
int LeftMin = 0;

int RightMax = 0;
int RightMin = 0; // We have a 10-bit analog resolution

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

void fullCalibration(uint8_t leftPhotoresistor, uint8_t rightPhotoresistor, int analogResolution, int calibrationTime) {
  /*
   * Calibrate both photo cells then return true to indicate success.
   * 
   * calibrationTime: Defaults to 5000 ms, simply sets the total time (in ms) we calibrate for.
   */
  analogReadResolution(analogResolution);
  RightMax = (1<<analogResolution);
  LeftMax = (1<<analogResolution);
  while (millis() < calibrationTime) {
    CalibratePhotoresistors(leftPhotoresistor, &LeftMax, &LeftMin);
    CalibratePhotoresistors(rightPhotoresistor, &RightMax, &RightMin);
  }
  calibrated = true;
}

void readRawPhotoCells(uint8_t LeftPhotoresistor, uint8_t RightPhotoresistor, photoVals *photoValues) {
  if (calibrated) {
    if (photoValues == NULL) {return;}
    (*photoValues).Left_Photo = analogRead(LeftPhotoresistor);
    (*photoValues).Right_Photo = analogRead(RightPhotoresistor);
  }
}

void processPhotoCells(photoVals *photoValues) {
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
  if (photoValues == NULL) {return;}
  (*photoValues).Left_Photo = constrain((*photoValues).Left_Photo, LeftMin, LeftMax);
  (*photoValues).Right_Photo = constrain((*photoValues).Right_Photo, RightMin, RightMax);
  
  (*photoValues).Left_Photo = map((*photoValues).Left_Photo, LeftMin, LeftMax, 0, 2); // Anything greater than 0 indicates a light surface, anything that is 0 indicates a dark surface
  (*photoValues).Right_Photo = map((*photoValues).Right_Photo, RightMin, RightMax, 0, 2); 
}

int handlePhotoCells(photoVals *photoValues, bool reverseColor) {
    if (photoValues == NULL) {return -1;}
    int Left_Photo = (*photoValues).Left_Photo;
    int Right_Photo = (*photoValues).Right_Photo;

    if ((Left_Photo && !Right_Photo) || (!Left_Photo && Right_Photo)) { // If we detect dark, it must exclusively be detected on one side (This is the XOR of Left_Photo and Right_Photo)
        if ((!Left_Photo && reverseColor) || (Left_Photo && !reverseColor)) { // Left Side is dark (like asphalt)
          // LEFT SIDE TRIGGERED (MEANING ROBOT WENT OFF THE LINE ON THE LEFT SIDE)
          // CALL FUNCTION HERE, LEAVE THE RETURN
          return 1;
        }
        else { // Right Side is dark (like asphalt)
          // RIGHT SIDE TRIGGERED (MEANING ROBOT WENT OFF THE LINE ON THE RIGHT SIDE)
          // CALL YOUR FUNCTION HERE, LEAVE THE RETURN
          return 2;
        }
    }
    else if ((Left_Photo && Right_Photo) && reverseColor || !(Left_Photo && Right_Photo) && !reverseColor) { // Since we reversed the colors, we know we are on the line if we detect white on both sides, thus turn off the LEDs
      // THIS IS THE CASE THERE WE ARE ON THE LINE AND HAVE NOT VEERED OFF.
      // CALL FUNCTION HERE
      return 0;
    }
    // IF WE END UP OUT HERE, AN ERROR OCCURRED, AT THE MOMENT THE ONLY WAY THE CODE CAN REACH THIS POINT IS BY DETECTING THAT WE ARE OFF THE LINE ON BOTH SIDES
    return -1;
    /*if (reverseColor) { // Light is the line, dark is everything else
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
  }*/
  //return -1;
}

void trackLine(uint8_t LeftPhotoresistor, uint8_t RightPhotoresistor, bool reverseColor, photoVals *photoValues) {
  readRawPhotoCells(LeftPhotoresistor, RightPhotoresistor, photoValues);
  processPhotoCells(photoValues);
  // YOU CAN COMMENT THE CODE BELOW OUT IF YOU WANT, YOU JUST NEED TO CALL handlePhotoCells(photoValues, reverseColor) AGAIN
  // BELOW MAY BE USEFUL FOR UNDERSTANDING WHICH PHOTO CELLS TRIGGERED
  switch (handlePhotoCells(photoValues, reverseColor)) {
    case 1: // Left side Triggered
      digitalWrite(RightLEDPin, HIGH);
      break;
    case 2:
      digitalWrite(LeftLEDPin, HIGH);
      break;
    case -1: // error occurred
      break;
    default:
      digitalWrite(RightLEDPin, LOW);
      digitalWrite(LeftLEDPin, LOW);
      break;
  }
}

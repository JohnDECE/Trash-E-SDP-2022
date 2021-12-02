#include "Photo_Cell_Calibrator.h"
#include "motor_control.h"

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

int readRawPhotoVal(uint8_t photoCellPin) {
  /*
   * Singular version of readRawPhotoCells, does the same thing except only for one photocell.
   */
  return analogRead(photoCellPin);
}

void readRawPhotoCells(uint8_t LeftPhotoresistor, uint8_t RightPhotoresistor, photoVals *photoValues) {
  /*
   * Fair self-explanatory: Reads raw data from both of our photo cells and stores into our photoVals struct which we have a pointer to.
   */
  if (calibrated) {
    if (photoValues == NULL) {return;}
    (*photoValues).Left_Photo = readRawPhotoVal(LeftPhotoresistor);
    (*photoValues).Right_Photo = readRawPhotoVal(RightPhotoresistor);
  }
}

void processPhotoCells(photoVals *photoValues) {
  /*
   * This function will take in a photoVals struct defined in Photo_Cell_Calibrator.h
   * 
   * It will then squeeze our newly read photoresistor values into our calibrated bounds and then further squeeze/map them into an integer value between 0 and 2.
   * 
   * The reason why this works is that we are working with high contrast colors, thus if we detect light this leads to a higher sensor value and since we are working with
   * high contrast, the difference between light and dark is significant. Thus our high sensor value will map into a non-zero integer which will thus act as our LOGIC HIGH boolean.
   * And vice versa, and dark color will then lead to a low sensor value which should map to zero, thus it will act as our LOGIC LOW boolean.
   * 
   * The reason why I choose 0 to 2 rather than 0 to 1, is because I wanted to reduce the mapping range for 0. Thus we are less susceptible to slightly dark colors triggering our low logic.
   * Why we might want this, is because our line might have dust or something on it and it might trigger the sensor to be mapped to 0. 
   * 
   * Otherwise, it will do nothing if there are no values to work with
   * 
   * 
   */
  if (photoValues == NULL) {return;}
  (*photoValues).Left_Photo = constrain((*photoValues).Left_Photo, LeftMin, LeftMax);
  (*photoValues).Right_Photo = constrain((*photoValues).Right_Photo, RightMin, RightMax);
  
  (*photoValues).Left_Photo = map((*photoValues).Left_Photo, LeftMin, LeftMax, 0, 2); // Anything greater than 0 indicates a light surface, anything that is 0 indicates a dark surface
  (*photoValues).Right_Photo = map((*photoValues).Right_Photo, RightMin, RightMax, 0, 2); 
}

int handlePhotoCells(photoVals *photoValues, bool reverseColor) {
  /*
   * This will determine which sensor triggered (Assumes 2 photo cells each representing either left or right)
   * Returns an integer:
   * 1 for left side triggered
   * 2 for right side triggered
   * 0 for nothing triggered (normal)
   * -1 for an unexpected case/error
   */
  if (photoValues == NULL) {return -1;}
  int Left_Photo = (*photoValues).Left_Photo;
  int Right_Photo = (*photoValues).Right_Photo;
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
  return -1;
}

void trackLine(uint8_t LeftPhotoresistor, uint8_t RightPhotoresistor, bool reverseColor, photoVals *photoValues, bool towards_pickup, uint8_t motorSpeed) {
  /*
   * This function will facilitate the reading and processing of our photo cell values. This function expects to read from 2 photo cells.
   * 
   * This function was written to work with Trash-E which is a vehicular robot, thus it accepts some other parameters to interact with other components of the robot.
   */
  readRawPhotoCells(LeftPhotoresistor, RightPhotoresistor, photoValues); 
  processPhotoCells(photoValues);
  // YOU CAN COMMENT THE CODE BELOW OUT IF YOU WANT, YOU JUST NEED TO CALL handlePhotoCells(photoValues, reverseColor) AGAIN
  // BELOW MAY BE USEFUL FOR UNDERSTANDING WHICH PHOTO CELLS TRIGGERED
  switch (handlePhotoCells(photoValues, reverseColor)) {
    case 1: // Left side Triggered
      digitalWrite(RightLEDPin, HIGH);
      if(towards_pickup)
      {
        //fullStop();
        rotateCW(motorSpeed+20);
        delay(50);
      }
      else
      {
       rotateCCW(motorSpeed+20);
       delay(50);
      }
      break;
    case 2:
      digitalWrite(LeftLEDPin, HIGH);
      if(towards_pickup)
      {
        //fullStop();
        rotateCCW(motorSpeed+20);
        delay(50);
      }
      else
      {
        rotateCW(motorSpeed+20);
        delay(50);
      }
      break;
    case -1: // error occurred
      break;
    default:
      if(towards_pickup)
      {
        forward(motorSpeed);
      }
      else
      {
        backward(motorSpeed);
      }
      digitalWrite(RightLEDPin, LOW);
      digitalWrite(LeftLEDPin, LOW);
      break;
  }
}

#include "Photo_Cell_Calibrator.h"
#include "motor_control.h"
#include "telegram_bot.h"

uint8_t AIN1 = 27;
uint8_t BIN1 = 32;
uint8_t AIN2 = 33;
uint8_t BIN2 = 14;
uint8_t PWMA = 15;
uint8_t PWMB = 13;
uint8_t STBY = 21;
uint8_t FORWARD = 1;
uint8_t BACKWARD = 0;
uint8_t STOP = 0;
uint8_t BEGIN = 1;
uint8_t ULTRA_DIRECTION = 26;
uint8_t ULTRA_DETECTION = 36;
uint8_t ULTRA_SLEEP = 25;
uint8_t RIGHT_PHOTO = 34;
uint8_t LEFT_PHOTO = 39;
uint8_t LEFT_LED = 12;
uint8_t RIGHT_LED = 4;


uint8_t motorSpeed = 80;
bool towards_pickup = true; // Forward is defined as towards pickup; 0 is going home
bool objectDetected;
bool messageSent = false;
bool atPickup = 0;
bool atHome = 1; // It will start at home


int analogResolution = 10;
photoVals photoValues;

void setup() {
  Serial.begin(115200);
  motorSetup(PWMA, PWMB, AIN1, AIN2, BIN1, BIN2, STBY);
  ultraSetup(ULTRA_DIRECTION, ULTRA_DETECTION, ULTRA_SLEEP);
  setupPhotoresistors(LEFT_LED, RIGHT_LED, LEFT_PHOTO, RIGHT_PHOTO, analogResolution);
  teleBotSetup();

}

void loop(){
  //rotateCCW(motorSpeed);
  objectDetected = digitalRead(ULTRA_DETECTION);
  if(towards_pickup && (!objectDetected))
  {
    digitalWrite(ULTRA_DIRECTION, HIGH); //This will tell the red board which ultrasonics to use
    trackLine(LEFT_PHOTO, RIGHT_PHOTO, true, &photoValues, 1, motorSpeed);
  }


  if((!towards_pickup) && (!objectDetected))
  {
    digitalWrite(ULTRA_DIRECTION, LOW); //This will tell the red board which ultrasonics to use
    trackLine(LEFT_PHOTO, RIGHT_PHOTO, true, &photoValues, 0, motorSpeed);
  }

  
  while(objectDetected)
  {
    fullStop();
    objectDetected = digitalRead(ULTRA_DETECTION);
    if (!messageSent) {
      alertUser("Object in the way! Please Remove it!");
      messageSent = true;
    }
  }
  if (messageSent) {
    messageSent = false;
    alertUser("All Clear!");
  }
  
  if(atPickup)
  {
    fullStop();
    //while(still trash){ wait for load sensor to indicate no trash; set atPickup low; set towards_pickup low}
  }
  if(atHome)
  {
    //wait until scheduled time... Maybe for MDR we can do a button push; set towards_pickup high
  }

  //atPickup = detectPickupColor(); have to write a function for which color is pickup/home
  //atHome = detectHomeColor();

  

}

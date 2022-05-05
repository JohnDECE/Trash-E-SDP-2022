#ifndef BATTERYREAD_H
#define BATTERYREAD_H
#include <Arduino.h>

//The default values are based on the adafruit feather huzzah 32 which uses 3.3V logic and automatically has an ADC resolution of 12 bits and the battery read pin is GPIO 35
void setupBatRead(uint8_t readPin=35, uint8_t analogResolution=12, float workVolt=3.3);
float batCHGRead();
#endif

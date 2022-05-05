#include "batteryread.h"

uint8_t batCHGPin = 0;

float ADCtoV = 0;

void setupBatRead(uint8_t readPin, uint8_t analogResolution, float workVolt) {
  batCHGPin = readPin;
  analogReadResolution(analogResolution);
  ADCtoV = workVolt / (1<<analogResolution);
  pinMode(batCHGPin, INPUT);
}

float batCHGRead() {
  if (batCHGPin && ADCtoV) {
    return analogRead(batCHGPin) * ADCtoV * 2;
  }
  return 0;
}

#ifndef SensorsSDP_H
#define SensorsSDP_H
#include <Arduino.h>

bool SonarSensor(int trigPinSensor, int echoPinSensor, int threshold, int LED = -1);

#endif

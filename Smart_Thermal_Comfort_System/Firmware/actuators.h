#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>

void actuator_begin();
void actuator_update();

extern String systemMode;   // "AUTO" | "MANUAL"
extern bool   fanState;
extern bool   peltierState;

#endif
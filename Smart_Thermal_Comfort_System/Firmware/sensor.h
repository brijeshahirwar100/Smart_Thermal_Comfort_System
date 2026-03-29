#ifndef SENSOR_H
#define SENSOR_H

void  sensor_begin();
void  sensor_update();

float sensor_getAmbient();
float sensor_getHumidity();
float sensor_getSkin();
float sensor_getTCI();        // centralised — no more duplicate formula
bool  sensor_getPresence();

#endif
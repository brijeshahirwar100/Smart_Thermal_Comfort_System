#include "telemetry.h"
#include "sensor.h"
#include "network.h"
#include "actuators.h"
#include "config.h"

static unsigned long lastSend = 0;

void telemetry_send() {
    if (millis() - lastSend < TELEMETRY_INTERVAL) return;  // FIX: now uses config constant
    lastSend = millis();

    if (!client.connected()) return;

    // FIX: TCI now from sensor_getTCI() — no duplicate formula
    String p = "{";
    p += "\"ambient_temp\":"  + String(sensor_getAmbient())  + ",";
    p += "\"humidity\":"      + String(sensor_getHumidity())  + ",";
    p += "\"skin_temp\":"     + String(sensor_getSkin())      + ",";
    p += "\"tci\":"           + String(sensor_getTCI())       + ",";
    p += "\"presence\":"      + String(sensor_getPresence() ? 1 : 0) + ",";
    p += "\"mode\":\""        + systemMode                    + "\",";
    p += "\"fan\":"           + String(fanState     ? 1 : 0)  + ",";
    p += "\"peltier\":"       + String(peltierState ? 1 : 0);
    p += "}";

    client.publish("v1/devices/me/telemetry", p.c_str());
}
#include "config.h"
#include "sensor.h"
#include "actuators.h"
#include "network.h"
#include "telemetry.h"
#include "debug.h"

static unsigned long lastDebug = 0;

void setup() {
    Serial.begin(9600);
    sensor_begin();
    actuator_begin();
    network_begin();
}

void loop() {
    sensor_update();
    actuator_update();
    network_loop();
    telemetry_send();

    if (millis() - lastDebug > 5000) {
        lastDebug = millis();
        debug_print();
    }
}
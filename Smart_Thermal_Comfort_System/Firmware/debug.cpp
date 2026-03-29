#include <Arduino.h>
#include "debug.h"
#include "sensor.h"
#include "actuators.h"
#include "config.h"

void debug_print() {
    Serial.println("====================================");

    Serial.print("Ambient: ");  Serial.print(sensor_getAmbient());  Serial.println(" C");
    Serial.print("Humidity: "); Serial.print(sensor_getHumidity()); Serial.println(" %");
    Serial.print("Skin:    ");  Serial.print(sensor_getSkin());     Serial.println(" C");
    Serial.print("TCI:     ");  Serial.println(sensor_getTCI());    // FIX: from getter, not recomputed

    Serial.print("Presence: ");
    Serial.println(sensor_getPresence() ? "DETECTED" : "NOT DETECTED");

    Serial.print("Mode: "); Serial.println(systemMode);

    Serial.print("Fan:     "); Serial.println(fanState     ? "ON" : "OFF");
    Serial.print("Peltier: "); Serial.println(peltierState ? "ON" : "OFF");

    // FIX: was inverted — INPUT_PULLUP means LOW = pressed
    Serial.print("Cool btn: ");
    Serial.println(digitalRead(BTN_COOL) == HIGH ? "PRESSED" : "RELEASED");

    Serial.print("Heat btn: ");
    Serial.println(digitalRead(BTN_HEAT) == HIGH ? "PRESSED" : "RELEASED");

    Serial.println("====================================\n");
}
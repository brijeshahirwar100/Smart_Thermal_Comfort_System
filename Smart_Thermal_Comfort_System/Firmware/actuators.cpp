#include <Arduino.h>
#include "actuators.h"
#include "sensor.h"
#include "config.h"

// ── GLOBALS ───────────────────────────────────────
String systemMode   = "AUTO";
bool   fanState     = false;
bool   peltierState = false;

// ── INIT ──────────────────────────────────────────
void actuator_begin() {
    pinMode(FAN_PIN,     OUTPUT);
    pinMode(PELTIER_PIN, OUTPUT);
}

// ── APPLY ─────────────────────────────────────────
static void applyOutputs() {
    digitalWrite  (FAN_PIN,     fanState     ? HIGH : LOW);   // FIX: was HIGH(=1) in AUTO mode
    digitalWrite (PELTIER_PIN, peltierState ? HIGH : LOW);
}

// ── UPDATE ────────────────────────────────────────
void actuator_update() {

    // Presence off → everything off
    if (!sensor_getPresence()) {
        fanState = peltierState = false;
        applyOutputs();
        return;
    }

    // Manual mode: outputs already set by sensor_update() buttons
    if (systemMode == "MANUAL") {
        applyOutputs();
        return;
    }

    // Auto mode: TCI-based control
    float tci = sensor_getTCI();

    if      (tci > 2) { fanState = true;  peltierState = false; }
    else if (tci < -2) { fanState = false; peltierState = true;  }
    else               { fanState = false; peltierState = false; }

    applyOutputs();
}
#include <Arduino.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "config.h"
#include "sensor.h"
#include "actuators.h"

// ── HARDWARE ──────────────────────────────────────
static DHT             dht(DHT_PIN, DHT_TYPE);
static OneWire         oneWire(DS18_PIN);
static DallasTemperature ds18(&oneWire);

// ── STATE ─────────────────────────────────────────
static float ambientTemp = 0.0;
static float humidity    = 0.0;
static float skinTemp    = 30.0;
static bool  presence    = false;

// ── INIT ──────────────────────────────────────────
void sensor_begin() {
    dht.begin();
    ds18.begin();
    pinMode(PRESENCE_PIN, INPUT);
    pinMode(BTN_COOL, INPUT);
    pinMode(BTN_HEAT, INPUT);
}

// ── UPDATE ────────────────────────────────────────
void sensor_update() {
    // DHT22
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if (!isnan(t)) ambientTemp = t;
    if (!isnan(h)) humidity    = h;

    // DS18B20
    ds18.requestTemperatures();
    float ds = ds18.getTempCByIndex(0);
    skinTemp = (ds != DEVICE_DISCONNECTED_C) ? ds : 30.0;

    // Presence
    presence = (digitalRead(PRESENCE_PIN) == HIGH);

    // Buttons — only act in MANUAL mode
    if (systemMode == "MANUAL") {
        if (digitalRead(BTN_COOL) == HIGH) {
            fanState = true;  peltierState = false;
        } else if (digitalRead(BTN_HEAT) == HIGH) {
            fanState = false; peltierState = true;
        }
    }
}

// ── GETTERS ───────────────────────────────────────
float sensor_getAmbient()  { return ambientTemp; }
float sensor_getHumidity() { return humidity; }
float sensor_getSkin()     { return skinTemp; }
bool  sensor_getPresence() { return presence; }

// TCI in one place — FIX: was duplicated in 3 files
// Formula: weighted skin + ambient only (humidity normalised included — different unit)
float sensor_getTCI() {
    return   (skinTemp-34.0 ) + 0.5 *( ambientTemp-24.0) + 1.5 * (humidity/100.0);


 
}
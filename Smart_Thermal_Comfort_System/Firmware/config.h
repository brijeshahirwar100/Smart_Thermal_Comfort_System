#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ── MQTT ──────────────────────────────────────────
#define MQTT_SERVER "mqtt.thingsboard.cloud"
#define TOKEN       "Write Your Token ID"

// ── SENSOR PINS ───────────────────────────────────
#define DHT_PIN      2
#define DHT_TYPE     DHT22
#define DS18_PIN     7

// ── INPUT PINS ────────────────────────────────────
#define PRESENCE_PIN 3
#define BTN_COOL     4
#define BTN_HEAT     8

// ── OUTPUT PINS ───────────────────────────────────
#define FAN_PIN      5
#define PELTIER_PIN  6

// ── TIMING ────────────────────────────────────────
#define TELEMETRY_INTERVAL 2000   // ms

#endif
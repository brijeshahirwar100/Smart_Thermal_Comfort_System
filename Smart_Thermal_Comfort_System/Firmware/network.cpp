#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

#include "config.h"
#include "actuators.h"
#include "network.h"

// ── HARDWARE ──────────────────────────────────────
byte           mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
EthernetClient ethClient;
PubSubClient   client(ethClient);

// ── RPC CALLBACK ──────────────────────────────────
static void callback(char* topic, byte* payload, unsigned int length) {
    // Build message string
    String msg;
    for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

    // Extract request ID from topic: "v1/devices/me/rpc/request/{id}"
    String topicStr(topic);
    String requestId = topicStr.substring(topicStr.lastIndexOf('/') + 1);

    // ── Handle commands ────────────────────────────
    if (msg.indexOf("setMode") != -1) {
        systemMode = (msg.indexOf("MANUAL") != -1) ? "MANUAL" : "AUTO";
        Serial.println("Mode: " + systemMode);
    }

    if (msg.indexOf("setFan") != -1) {
        if (systemMode == "MANUAL") {
            fanState = (msg.indexOf("true") != -1);
            Serial.println("Fan: " + String(fanState ? "ON" : "OFF"));
        } else {
            Serial.println("Fan ignored (AUTO mode)");
        }
    }

    if (msg.indexOf("setPeltier") != -1) {
        if (systemMode == "MANUAL") {
            peltierState = (msg.indexOf("true") != -1);
            Serial.println("Peltier: " + String(peltierState ? "ON" : "OFF"));
        } else {
            Serial.println("Peltier ignored (AUTO mode)");
        }
    }

    // ── RPC response (required by ThingsBoard) ─────
    // Without this the dashboard button stays "pending"
    String responseTopic = "v1/devices/me/rpc/response/" + requestId;
    client.publish(responseTopic.c_str(), "{\"success\":true}");
}

// ── RECONNECT (non-blocking) ───────────────────────
static unsigned long lastRetry = 0;

static void reconnect() {
    if (millis() - lastRetry < 2000) return;
    lastRetry = millis();

    if (client.connect("Node1", TOKEN, NULL)) {
        client.subscribe("v1/devices/me/rpc/request/+");
        Serial.println("MQTT connected");
    }
}

// ── INIT ──────────────────────────────────────────
void network_begin() {
    Ethernet.begin(mac);
    delay(1000);
    client.setServer(MQTT_SERVER, 1883);
    client.setCallback(callback);
}

// ── LOOP ──────────────────────────────────────────
void network_loop() {
    if (!client.connected()) reconnect();
    client.loop();
}
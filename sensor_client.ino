#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
WebSocketsClient webSocket;
 
const char *ssid     = "Neurolabs";
const char *password = "neuroTechlab@iith";
 
unsigned long messageInterval = 1;
bool connected = false;
DHT dht(DHTPIN, DHTTYPE);
#define DEBUG_SERIAL Serial
 
void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
    const uint8_t* src = (const uint8_t*) mem;
    DEBUG_SERIAL.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
    for(uint32_t i = 0; i < len; i++) {
        if(i % cols == 0) {
            DEBUG_SERIAL.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
        }
        DEBUG_SERIAL.printf("%02X ", *src);
        src++;
    }
    DEBUG_SERIAL.printf("\n");
}
 
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
 
    switch(type) {
        case WStype_DISCONNECTED:
            DEBUG_SERIAL.printf("[WSc] Disconnected!\n");
            connected = false;
            break;
        case WStype_CONNECTED: {
            DEBUG_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
            connected = true;
 
            // send message to server when Connected
            DEBUG_SERIAL.println("[WSc] SENT: Connected");
            webSocket.sendTXT("Connected");
        }
            break;
        case WStype_TEXT:
            DEBUG_SERIAL.printf("[WSc] RESPONSE: %s\n", payload);
            break;
        case WStype_BIN:
            DEBUG_SERIAL.printf("[WSc] get binary length: %u\n", length);
            hexdump(payload, length);
            break;
        case WStype_PING:
            // pong will be send automatically
            DEBUG_SERIAL.printf("[WSc] get ping\n");
            break;
        case WStype_PONG:
            // answer to a ping we send
            DEBUG_SERIAL.printf("[WSc] get pong\n");
            break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    }
}
 
void setup() {
    DEBUG_SERIAL.begin(115200);
 
//  DEBUG_SERIAL.setDebugOutput(true);
 
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
    pinMode(34, INPUT);
    pinMode(35, INPUT);
    pinMode(36, INPUT);
    pinMode(39, INPUT);
    for(uint8_t t = 4; t > 0; t--) {
        DEBUG_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        DEBUG_SERIAL.flush();
        delay(1000);
    }
    dht.begin();
    WiFi.begin(ssid, password);
 
    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      DEBUG_SERIAL.print ( "." );
    }
    DEBUG_SERIAL.print("Local IP: "); DEBUG_SERIAL.println(WiFi.localIP());
    // server address, port and URL
    webSocket.begin("192.168.0.237", 7891, "/");
 
    // event handler
    webSocket.onEvent(webSocketEvent);
}
 
unsigned long lastUpdate = millis();
 
 
void loop() {
    webSocket.loop();
    if (connected && lastUpdate+messageInterval<millis()){
        
        float temp = dht.readTemperature();
        float pin1 = analogRead(34);
        float pin2 = analogRead(35);
        float pin3 = analogRead(36);
        float pin4 = analogRead(39);
        DynamicJsonDocument doc(2048);
        doc["temp"] = temp;
        doc["pin1"] = pin1;
        doc["pin2"] = pin2;
        doc["pin3"] = pin3;
        doc["pin4"] = pin4;
        String json;
        serializeJson(doc, json);
        Serial.println(temp);
        DEBUG_SERIAL.println("[WSc] SENT: Simple js client message!!");
        webSocket.sendTXT(json);
        lastUpdate = millis();
    }
}

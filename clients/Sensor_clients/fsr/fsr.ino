//Green ESP32
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
WebSocketsClient webSocket;

#define FSR1 35
#define FSR2 34

const char* ssid = "Neurolabs";
const char* password = "neuroTechlab@iith";
 
unsigned long messageInterval = 1;
bool connected = false;
#define DEBUG_SERIAL Serial

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
 
    switch(type) {
        case WStype_DISCONNECTED:
            DEBUG_SERIAL.printf("[WSc] Disconnected!\n");
            connected = false;
            break;
        case WStype_CONNECTED: {
            //DEBUG_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
            connected = true;
 
            // send message to server when Connected
            DEBUG_SERIAL.println("[WSc] SENT: Connected");
        }
            break;
        case WStype_TEXT:
            //DEBUG_SERIAL.printf("[WSc] RESPONSE: %s\n", payload);
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
    DEBUG_SERIAL.begin(9600);
    pinMode(FSR1, INPUT); // initialize the sensor
    pinMode(FSR2, INPUT); // initialize the sensor  
   
    //  DEBUGs_SERIAL.setDebugOutput(true);
 
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
    DEBUG_SERIAL.println();
    for(uint8_t t = 4; t > 0; t--) {
        DEBUG_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        DEBUG_SERIAL.flush();
        delay(1000);
    }
    WiFi.begin(ssid, password);
 
    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      DEBUG_SERIAL.print ( "." );
    }
    DEBUG_SERIAL.print("Local IP: "); DEBUG_SERIAL.println(WiFi.localIP());
    // server address, port and URL
    webSocket.begin("192.168.0.237", 7891);
 
    // event handler
    webSocket.onEvent(webSocketEvent);
}
 
unsigned long lastUpdate = millis();
 
 
void loop() {
    webSocket.loop();
    if (connected && lastUpdate+messageInterval<millis()){
        float fsr1 = analogRead(FSR1);
        float fsr2 = analogRead(FSR2);
        Serial.println(fsr1-fsr2);
        DynamicJsonDocument doc(2048);
        doc["sensor_type"] = 1;
        doc["fsr"] = fsr1-fsr2;
        doc["time"] = millis();

        String json;
        //DEBUG_SERIAL.println(doc.memoryUsage());
        serializeJson(doc, json); 
        
        //DEBUG_SERIAL.println("[WSc] SENT: Simple js client message!!");
        
        webSocket.sendTXT(json);
        lastUpdate = millis();
    }
} 

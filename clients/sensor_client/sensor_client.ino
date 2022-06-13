#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
WebSocketsClient webSocket;
int rows = 5;
int columns = 5;

int i,j, sensor;
int f = 0;
int Sin[4] = {19, 18, 4, 2};
int Sout[4] = {27,14,12,13};
int MUXtable[16][4] =
{
  {0,0,0,0},{0,0,0,1},{0,0,1,0},{0,0,1,1},
  {0,1,0,0},{0,1,0,1},{0,1,1,0},{0,1,1,1},
  {1,0,0,0},{1,0,0,1},{1,0,1,0},{1,0,1,1},
  {1,1,0,0},{1,1,0,1},{1,1,1,0},{1,1,1,1},
};
int sig_input = 26;
int sig_output = 34;

const char *ssid     = "TP-Link_B268";
const char *password = "14804120";
 
unsigned long messageInterval = 1;
bool connected = false;
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
    pinMode(sig_input, OUTPUT); //giving output to multiplexer
    pinMode(sig_output, INPUT); //taking input from the multiplexer
    for(i = 0; i<4; i++) pinMode(Sin[i], OUTPUT);
    for(i = 0; i<4; i++) pinMode(Sout [i], OUTPUT);
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
    webSocket.begin("192.168.0.100", 7891, "/yoga_mat");
 
    // event handler
    webSocket.onEvent(webSocketEvent);
}
 
unsigned long lastUpdate = millis();
 
 
void loop() {
    webSocket.loop();
    if (connected && lastUpdate+messageInterval<millis()){
        int k=0;
        DynamicJsonDocument doc(2048);
        for(j = 0; j<columns ; j++){
        digitalWrite(Sin[0], MUXtable[j][0]);
        digitalWrite(Sin[1], MUXtable[j][1]);
        digitalWrite(Sin[2], MUXtable[j][2]);
        digitalWrite(Sin[3], MUXtable[j][3]);
        for(int i =0; i<rows ; i++){
          digitalWrite(Sout[0], MUXtable[j][0]);
          digitalWrite(Sout[1], MUXtable[j][1]);
          digitalWrite(Sout[2], MUXtable[j][2]);
          digitalWrite(Sout[3], MUXtable[j][3]);
          sensor = analogRead(sig_output);
          doc["Data"][k++] = sensor;
        }
       }  
     
        String json;
        DEBUG_SERIAL.println(doc.memoryUsage());
        serializeJson(doc, json);
        DEBUG_SERIAL.println("[WSc] SENT: Simple js client message!!");
        
        webSocket.sendTXT(json);
        lastUpdate = millis();
    }
}

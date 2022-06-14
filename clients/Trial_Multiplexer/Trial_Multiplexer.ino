#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
WebSocketsClient webSocket;

int i,j;
int sensor;
int rows = 16;
int columns = 16;
int Sin[4] = {15,2,4,16};
int Sout[4] = {5,18,19,21};
int MUXtable[16][4] =
{
  {0,0,0,0},{0,0,0,1},{0,0,1,0},{0,0,1,1},
  {0,1,0,0},{0,1,0,1},{0,1,1,0},{0,1,1,1},
  {1,0,0,0},{1,0,0,1},{1,0,1,0},{1,0,1,1},
  {1,1,0,0},{1,1,0,1},{1,1,1,0},{1,1,1,1},
};
int sig_input = 17;
int sig_output = 34;

const char *ssid     = "Neurolabs";
const char *password = "neuroTechlab@iith";
 

bool connected = false;
unsigned long lastUpdate = millis();
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
            connected = true;
 
            // send message to server when Connected
            DEBUG_SERIAL.println("[WSc] SENT: Connected");
            
        }
            break;
        case WStype_TEXT:
            
            break;
        case WStype_BIN:
      
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
    webSocket.begin("192.168.0.237", 7891, "/");
 
    // event handler
    webSocket.onEvent(webSocketEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
   webSocket.loop();
    if (connected){
        int k=0;
        DynamicJsonDocument doc(10000);
        String json;
        digitalWrite(sig_input, HIGH);
        for(j = 0; j<columns ; j++){
          digitalWrite(Sin[0], MUXtable[j][0]);
          digitalWrite(Sin[1], MUXtable[j][1]);
          digitalWrite(Sin[2], MUXtable[j][2]);
          digitalWrite(Sin[3], MUXtable[j][3]);
          for(int i =0; i<rows ; i++){
            digitalWrite(Sout[0], MUXtable[i][0]);
            digitalWrite(Sout[1], MUXtable[i][1]);
            digitalWrite(Sout[2], MUXtable[i][2]);
            digitalWrite(Sout[3], MUXtable[i][3]);
            sensor = analogRead(sig_output);
            //Serial.print(sensor);
            doc["Data"][k++] = sensor;
            //Serial.print(" ");
           }  
           //Serial.println(" ");
        } 
        //Serial.println(" ");
        //Serial.println(" ");
        
        serializeJson(doc, json);
        
        webSocket.sendTXT(json);
        
    }
}

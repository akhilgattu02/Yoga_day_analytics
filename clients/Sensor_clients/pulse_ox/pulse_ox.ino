/*
  Optical Heart Rate Detection (PBA Algorithm) using the MAX30105 Breakout
  By: Nathan Seidle @ SparkFun Electronics
  Date: October 2nd, 2016
  https://github.com/sparkfun/MAX30105_Breakout

  This is a demo to show the reading of heart rate or beats per minute (BPM) using
  a Penpheral Beat Amplitude (PBA) algorithm.

  It is best to attach the sensor to your finger using a rubber band or other tightening
  device. Humans are generally bad at applying constant pressure to a thing. When you
  press your finger against the sensor it varies enough to cause the blood in your
  finger to flow differently which causes the sensor readings to go wonky.

  Hardware Connections (Breakoutboard to Arduino):
  -5V = 5V (3.3V is allowed)
  -GND = GND
  -SDA = A4 (or SDA)
  -SCL = A5 (or SCL)
  -INT = Not connected

  The MAX30105 Breakout can handle 5V or 3.3V I2C logic. We recommend powering the board with 5V
  but it will also run at 3.3V.
*/
#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
WebSocketsClient webSocket;



 //Time at which the last beat occurred




const char *ssid     = "Neurolabs";
const char *password = "neuroTechlab@iith";

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
            //DEBUG_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
            connected = true;
 
            // send message to server when Connected
            DEBUG_SERIAL.println("[WSc] SENT: Connected");
            
        }
            break;
        case WStype_TEXT:
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
 

void setup()
{
  Serial.begin(9600);
  pinMode(34, INPUT); 
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
    

  
  Serial.println("Place your index finger on the sensor with steady pressure.");

   //Turn off Green LED 
  DEBUG_SERIAL.print("Local IP: "); DEBUG_SERIAL.println(WiFi.localIP());
    // server address, port and URL
    webSocket.begin("192.168.0.237", 7891);
 
    // event handler
    webSocket.onEvent(webSocketEvent);
}

unsigned long lastUpdate = millis();

void loop()
{

  webSocket.loop();
  
  if (connected && lastUpdate+messageInterval<millis()){
    
    //We sensed a beat!
    
      //Take average o
      float gsr = analogRead(34);
      DynamicJsonDocument doc(5000);
      doc["sensor_type"] = 0; 
      doc["pulse"] = 60;
      doc["gsr"] = gsr;
      doc["time"] = millis();
      String json;
      serializeJson(doc, json);
      DEBUG_SERIAL.println("[WSc] SENT: Simple js client message!!");
      webSocket.sendTXT(json);
      lastUpdate = millis();
  
  }
    

}

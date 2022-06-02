int pin1 = 34;
int pin2 = 35;
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
const char* ssid = "Neurolabs";
const char* password = "neuroTechlab@iith";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.0.237:5000/fsr_sensor";

void setup() {
  Serial.begin(115200);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT); // initialize the sensor
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
}

void loop() {
  // wait a few seconds between measurements.

  // read humidity
  float reading_one = analogRead(pin1);
  float reading_two = analogRead(pin2);
  float diff = reading_two - reading_one;
  Serial.println(diff);
  if(WiFi.status()== WL_CONNECTED){
      DynamicJsonDocument doc(2048);
      doc["pin1"] = reading_one;
      doc["pin2"] = reading_two;
      doc["diff"] = diff;
// Serialize JSON document
      String json;
      serializeJson(doc, json);
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);
  // read temperature as Celsiu
  // read temperature as Fahrenheit
      
  // check if any reads failed
      
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(json);
      http.end();
  }
}

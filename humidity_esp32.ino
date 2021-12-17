#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <FreeRTOS.h>
 #include <DHT.h>
#define DHTPIN 19 

const char *SSID = "OPPO A5s";
const char *PWD = "92396527";

 
// Web server running on port 80
WebServer server(80);
 
// Sensor
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
 
// JSON data buffer
StaticJsonDocument<250> jsonDocument;
char buffer[250];
 
// env variable
float temperature;
float humidity;

 
void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    // we can even make the ESP32 to sleep
  }
 
  Serial.print("Connected. IP: ");
  Serial.println(WiFi.localIP());
}

void setup_routing() {     
  server.on("/temperature", getTemperature);        
  server.on("/humidity", getHumidity);     
  server.on("/env", getEnv);     
    
       
  // start server    
  server.begin();    
}
 
void create_json(char *tag, float value, char *unit) {  
  jsonDocument.clear();  
  jsonDocument["type"] = tag;
  jsonDocument["value"] = value;
  jsonDocument["unit"] = unit;
  serializeJson(jsonDocument, buffer);
}
 
void add_json_object(char *tag, float value, char *unit) {
  JsonObject obj = jsonDocument.createNestedObject();
  obj["type"] = tag;
  obj["value"] = value;
  obj["unit"] = unit; 
}

void read_sensor_data(void * parameter) {
   for (;;) {
     temperature = dht.readTemperature();
     humidity = dht.readHumidity();
     Serial.println("Read sensor data");
 
     // delay the task
     vTaskDelay(60000 / portTICK_PERIOD_MS);
   }
}
 
void getTemperature() {
  Serial.println("Get temperature");
  create_json("temperature", temperature, "°C");
  server.send(200, "application/json", buffer);
}
 
void getHumidity() {
  Serial.println("Get humidity");
  create_json("humidity", humidity, "%");
  server.send(200, "application/json", buffer);
}
 

 
void getEnv() {
  Serial.println("Get env");
  jsonDocument.clear();
  add_json_object("temperature", temperature, "°C");
  add_json_object("humidity", humidity, "%");
  serializeJson(jsonDocument, buffer);
  server.send(200, "application/json", buffer);
}

void handlePost() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  
  // Respond to the client
  server.send(200, "application/json", "{}");
}

void setup_task() {    
  xTaskCreate(     
  read_sensor_data,      
  "Read sensor data",      
  1000,      
  NULL,      
  1,     
  NULL     
  );     
}
void setup() {     
  Serial.begin(9600);    
     
  // Sensor setup    
  dht.begin();
  connectToWiFi();     
  setup_task();    
  setup_routing();     

}    
       
void loop() {    
  server.handleClient();     
}

#include <WiFiMulti.h>
#include <HTTPClient.h> 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

WiFiMulti wifiMulti;

#define LED_BUILTIN 2   // Set the GPIO pin where you connected your test LED or comment this line out if your dev board has a built-in LED

// Set these to your desired credentials.

const char* ssid     = "........";
const char* password = "..............";
//String SERVER_IP     = "http://192.168.43.21:8000";
//String SERVER_IP     = "http://192.168.2.105:8000";
String SERVER_IP     = "http://192.168.0.169:PORT NUMBER";
 
String GET_PING      = "/ping";
String POST_SENSOR   = "/ingest";
 

void getServerPing();
void readSensorData();
void postSensorData();


 float X,Y,Z;
WiFiServer server(80);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println();
  Serial.println("Trying ADXL345 sensor...");
  if(!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  //accel.setRange(ADXL345_RANGE_2_G);
  Serial.println("Configuring wifi...");
  // You can remove the password parameter if you want the AP to be open.
  wifiMulti.addAP(ssid, password); 
  Serial.println("Starting client ...");
  
}

void loop(){
  // wait for WiFi connection
  if((wifiMulti.run() == WL_CONNECTED)) { 
    Serial.println("Connected to Access Point\n");
    readSensorData();
    getServerPing();
     delay(2000); 
     postSensorData();
  } else {
    Serial.println(".");
  }
  delay(3000); 
}

void getServerPing(){
  HTTPClient http;
  Serial.println("[HTTP]  .\n"); 
  http.begin(SERVER_IP + GET_PING); //HTTP   htttp://127.0.0.1:8000/ping
  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  //Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
      }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    } 
    http.end();
}


void readSensorData()
{
  
  sensors_event_t event; 
  accel.getEvent(&event);
 
  X=event.acceleration.x;
  Y=event.acceleration.y;
  Z=event.acceleration.z;
  
  Serial.print("x:"); 
  Serial.print(event.acceleration.x);
  Serial.print(",");
  Serial.print("y:"); 
  Serial.print(event.acceleration.y); 
  Serial.print(",");
   Serial.print("z:"); 
 Serial.println(event.acceleration.z);
  delay(1000);

}
void postSensorData(){
  HTTPClient http;
  char json_string[100];
  Serial.println("[HTTP] begin...\n");
  http.begin(SERVER_IP + POST_SENSOR); //HTTP
  Serial.print("[HTTP] POST...\n");
       
  http.addHeader("Content-Type", "application/json");


 sprintf(json_string,"{\"SensorId\":\"Acc_Sensor\",\"data\": {\"XCoordinate\":%.02f,\"YCoordinate\":%.02f,\"ZCoordinate\":%.02f}}",X,Y,Z);
  // String jsonPayload = "{\"sensorId\":\"Acc_Sensor\",\"sensor\":\"ADXL345\",\"X Coordinate\":%f,\"Y Coordinate\":%f,\"Z Coordinate\":%f";
  int httpResponseCode = http.POST(json_string);
  //int httpResponseCode = http.POST(jsonPayload);
  // httpResponseCode will be negative on error
  if(httpResponseCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] POST... code: %d\n", httpResponseCode);

    // file found at server
    if(httpResponseCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
    }
  } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpResponseCode).c_str());
  } 
  http.end(); 
}  

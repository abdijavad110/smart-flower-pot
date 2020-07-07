#include <Adafruit_ESP8266.h>
#include <ESP8266HTTPClient.h>



#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>


#include <ESP8266WiFi.h>

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://192.168.1.106:1880/update-sensor";


#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define self_id = 1;      //board id(for server authorization) 

#define DHTPIN D8
#define DHTTYPE    DHT11 
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;


#define soil_humidity_enable D0
#define photocell_enable D1

#define moist_photocell_input = A0


void setup() {
  Serial.begin(9600);
  
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  int temperature_value;
  int soil_humidity_value;
  int photocell_value;

  pinMode(soil_humidity_enable, OUTPUT);
  pinMode(photocell_enable, OUTPUT);

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


  //get temperature from dht11 sensor
  delay(delayMS);
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    continue;
  }
  else {
    temperature_value = event.temperature;
  }

  //wait for 5 seconds
  delay(5000); 

  //get soil_humidity from input
  digitalWrite(soil_humidity_enable, HIGH); 
  soil_humidity_value = analogRead(moist_photocell_input);
  digitalWrite(soil_humidity_enable, LOW);
  delay(2000);

  //get photocell value from input
  digitalWrite(photocell_enable, HIGH); 
  photocell_value = analogRead(moist_photocell_input);
  digitalWrite(soil_humidity_enable, LOW);
  delay(2000);  


  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    char json_msg[200];
    sprintf(json_msg, "{\"board_id\":%d,\"temperature\": %d,\"soil_humidity\":%d,\"photocell\":%d}",
                                     self_id, temperature_value, soil_humidity_value, photocell_value);
    String httpRequestData = json_msg;

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

    if(httpResponseCode>0){
      String response_payload = http.getString();
      Serial.println(response_payload);             //todo: read server commands and activate actuators
    }

    http.end();
     

      
  }  



  
  
  

  












  

  // ==================soil humidity sensor=====================
  HS_val = analogRead(HS_Pin);
  Serial.print("soil humidity: ");  
  Serial.println(HS_val);                                   //todo: send to raspberry pi
  //(sensorval>1000: sensor disconnected or is not in the soil)
  //(600<sensorval<1000: soil is dry)
  //(370<sensorval<600: soil is humid)
  //(sensorval<370: sensor in water)
  



  // ==================temperature sensor========================

  int temp = analogRead(tmp);
  float volts = (temp / 965.0) * 5;
  float c = (volts - 0.5) * 100;

  Serial.print("temperature(in centigrade: )");
  Serial.println(c);



  // ==================light sensor========================
  
  PC_val = analogRead(PC_Pin);

  Serial.print("photocell output: )");
  Serial.println(PC_val);

 // PC_val < 10: Dark
 // 10<PC_val<200: Dim
 // 200<PC_val<500: Light
 // 500<PC_val<800: Bright
 // 800<PC_val: Very Bright


  delay(60000); 

}

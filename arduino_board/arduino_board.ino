#include <ArduinoJson.h>

#include <ESP8266HTTPClient.h>


#include <WiFiClient.h>
#include <WiFiServer.h>


#include <ESP8266WiFi.h>

char* ssid = "Irancell-TD-TK2510-8430";
char* password = "8718523e77";

//Your Domain name with URL path or IP address with path
char* serverName = "http://192.168.1.200:8080/agent";


#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define self_id = 1;      //board id(for server authorization) 

#define DHTPIN D2
#define DHTTYPE    DHT11 
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;


#define soil_humidity_enable D0
#define photocell_enable D1

#define moist_photocell_input A0

#define red D7
#define yellow D8



int temperature_value;
int soil_humidity_value;
int photocell_value;

int setTemperature = 0;
int setHumidity = 0;
int setBrightness = 0;

StaticJsonDocument<400> doc;


void setup() {
  Serial.begin(9600);
  
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
  

  pinMode(soil_humidity_enable, OUTPUT);
  pinMode(photocell_enable, OUTPUT);


  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);

  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  
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
    return;
  }
  else {
    temperature_value = event.temperature;
  }

  //wait for 5 seconds
  delay(2000); 

  //get soil_humidity from input
  digitalWrite(soil_humidity_enable, HIGH); 
  delay(2000);
  soil_humidity_value = analogRead(moist_photocell_input);
  digitalWrite(soil_humidity_enable, LOW);

  //get photocell value from input
  digitalWrite(photocell_enable, HIGH); 
  delay(2000);  
  photocell_value = analogRead(moist_photocell_input);
  digitalWrite(photocell_enable, LOW);


  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;

    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/json");

    char json_msg[200];

    //scale sensor values to same variables in server
    soil_humidity_value = (soil_humidity_value*95/1024)+5;
    photocell_value = (photocell_value*5000/1024)+1000;
    
    sprintf(json_msg, "{\"humidity\":%d,\"temperature\":%d,\"brightness\":%d}",
                                     soil_humidity_value, temperature_value, photocell_value);
    String httpRequestData = json_msg;

    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    if(httpResponseCode>0){
      String response_payload = http.getString();
      Serial.println(response_payload);
      DeserializationError error = deserializeJson(doc, response_payload);
      if (error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      setTemperature = doc["setTemperature"];
      setHumidity = doc["setHumidity"];
      setBrightness = doc["setBrightness"];
    }
    http.end(); 
  }

  
  // activate actuators
  if ((setHumidity - soil_humidity_value)>20){
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
    delay(2000);
    digitalWrite(yellow, LOW);
    digitalWrite(red, HIGH);
  }

  if (setBrightness>=2000){
    if(setBrightness<3000){
      digitalWrite(yellow, HIGH);
      digitalWrite(red, LOW);
    }  
    else{
      digitalWrite(yellow, HIGH);
      digitalWrite(red, HIGH);
    }
  }
  if(setBrightness<2000){
      digitalWrite(yellow, LOW);
      digitalWrite(red, HIGH);
  }

      

}

#define HS_Pin A0             // HS: Humidity Sensor
int HS_val = 0;

int PC_Pin = 0;               // PC: Photocell
int PC_val;

void setup() {
  // put your setup code here, to run once:
  const int tmp = A1;
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

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

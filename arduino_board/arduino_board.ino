#include <Arduino_FreeRTOS.h>


TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;

void setup() {
Serial.begin(9600);
Serial.println("In setup function.");

pinMode(3, INPUT);
pinMode(9, OUTPUT);

attachInterrupt(digitalPinToInterrupt(3), ExternalInterrupt, CHANGE);

xTaskCreate(MyTask2, "Task2", 100, NULL, 2, &TaskHandle_2);
xTaskCreate(MyTask3, "Task3", 100, NULL, 3, &TaskHandle_3);
}

void loop() {
// Idle task, it will run whenever CPU is idle.
//Serial.println("Loop function");
//delay(1000);
}

static void ExternalInterrupt(){

Serial.println("ISR starting task2");
xTaskResumeFromISR(TaskHandle_2);
Serial.println("Leaving ISR.");
taskYIELD();
}

static void MyTask2(void *pvParameters){
Serial.println("Task 2 started.");

for(int i=0; i<10; i++){
if(i%2 == 0){
digitalWrite(9, HIGH);
vTaskDelay( 1000 / portTICK_PERIOD_MS );
}
else{
digitalWrite(9, LOW);
vTaskDelay( 1000 / portTICK_PERIOD_MS );
}
}
Serial.println("Task 2 finished.");
vTaskSuspend(NULL);
}

static void MyTask3(void *pvParameters){
Serial.println("Task3 running, suspending all tasks.");
vTaskSuspend(TaskHandle_2);
vTaskDelete(NULL);
}

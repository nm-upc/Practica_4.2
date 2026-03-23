#include <Arduino.h>

SemaphoreHandle_t semOn;
SemaphoreHandle_t semOff;

void TaskLEDOn(void *pvParameters);
void TaskLEDOff(void *pvParameters);

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  semOn = xSemaphoreCreateBinary();
  semOff = xSemaphoreCreateBinary();

  xTaskCreatePinnedToCore(TaskLEDOn, "LED ON", 2048, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(TaskLEDOff, "LED OFF", 2048, NULL, 1, NULL, 1);

  xSemaphoreGive(semOn); // Empieza la tarea de encender
}

void loop() {
}

void TaskLEDOn(void *pvParameters) {
  for(;;) {
    xSemaphoreTake(semOn, portMAX_DELAY);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED ON");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xSemaphoreGive(semOff);
  }
}

void TaskLEDOff(void *pvParameters) {
  for(;;) {
    xSemaphoreTake(semOff, portMAX_DELAY);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED OFF");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    xSemaphoreGive(semOn);
  }
}
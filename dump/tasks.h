// ***** tasks.h *****
#ifndef TASKS_H
#define TASKS_H

#include <Arduino_FreeRTOS.h>

//======================================================================//
// Prototypes

void setupTasks();
void DebitTask(void *);
void BlinkTask(void *);

//======================================================================//
// Functions

void setupTasks() {
  xTaskCreate(DebitTask, "DebitTask", 2048, NULL, 1, NULL);
  xTaskCreate(BlinkTask, "BlinkTask", 2048, NULL, 1, NULL);
}

void DebitTask(void *param) {
   (void) param;
   // Serial.println("debitTask: Executing on core ");
   
    for(;;) {
        // printdebitCountTemporary();
        // Serial.print("debitFrequency: ");
        Serial.println(debitFrequency);
        vTaskDelay(5000 / portTICK_PERIOD_MS); // Delay between loops to reset watchdog timer portMAX_DELAY
    }
   vTaskDelete(NULL);
}

void BlinkTask(void *param) {
   (void) param;
   pinMode(LED_BUILTIN, OUTPUT);
   for(;;) {
     digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
     Serial.println("On");
     vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
     digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
     Serial.println("Off");
     vTaskDelay( 1000 / portTICK_PERIOD_MS ); // wait for one second
 }
   vTaskDelete(NULL);
}

#endif // TASKS_H
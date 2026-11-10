#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include <semphr.h>

SemaphoreHandle_t semaphore;

#define MAIN_TASK_PRIORITY (tskIDLE_PRIORITY + 4UL)
#define HIGH_PRIORITY_TASK_PRIORITY (tskIDLE_PRIORITY + 3UL)
#define MEDIUM_PRIORITY_TASK_PRIORITY (tskIDLE_PRIORITY + 2UL)
#define LOW_PRIORITY_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define PRIORITY_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void low_priority(__unused void *params) {
  if (xSemaphoreTake(semaphore, (TickType_t)10) == pdTRUE) {
    printf("low priority function got the sem");
    vTaskDelay(1000 / portTICK_RATE_MS);
    xSemaphoreGive(semaphore);
    printf("low priority funciton gave the sem");
  } else {
    printf("Error low priority task can't grab sem");
  }
  while (1) {
    vTaskDelay(1 / portTICK_RATE_MS);
  }
}

void medium_priority(__unused void *params) 
{
    printf("Medium priority task attempting to starve system.\n");
    while (1)
    {

    }
}

void high_priority(__unused void *params) 
{ 
    vTaskDelay(500);

    if (xSemaphoreTake(semaphore, 5000) == pdTRUE) {
        printf("high priority function got the sem");
        xSemaphoreGive(semaphore);
    } else {
        printf("high priority task can't grab sem");
    }

    while (1) {
        vTaskDelay(500);
    }

}

void supervisor_task(__unused void *params) {
  hard_assert(cyw43_arch_init() == PICO_OK);

  xTaskCreate(low_priority, "LowPriorityTask", PRIORITY_TASK_STACK_SIZE, NULL,
              LOW_PRIORITY_TASK_PRIORITY, NULL);

  vTaskDelay(100);

  xTaskCreate(medium_priority, "MediumPriorityTask", PRIORITY_TASK_STACK_SIZE,
              NULL, MEDIUM_PRIORITY_TASK_PRIORITY, NULL);

  xTaskCreate(high_priority, "HighPriorityTask", PRIORITY_TASK_STACK_SIZE, NULL,
              HIGH_PRIORITY_TASK_PRIORITY, NULL);

  while (true) {
    vTaskDelay(portMAX_DELAY);
  }
}

int main(void) {

  stdio_init_all();
  sleep_ms(5000); // Give time for screen to work

  semaphore = xSemaphoreCreateBinary();
  xSemaphoreGive(semaphore);

  TaskHandle_t task;

  xTaskCreate(supervisor_task, "Supervisor_thread",
              MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);

  vTaskStartScheduler();
  return 0;
}

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

SemaphoreHandle_t semaphore;


void low_priority(__unused void *params) {
    
}

void medium_priority(__unused void *params) {
    
}

void high_priority(__unused void *params) {
    
}


void supervisor_task(__unused void *params) {
    hard_assert(cyw43_arch_init() == PICO_OK);

    xTaskCreate(low_priority, "LowPriorityTask",
                PRIORITY_TASK_STACK_SIZE, NULL, LOW_PRIORITY_TASK_PRIORITY, &task);

    xTaskCreate(medium_priority, "MediumPriorityTask",
                PRIORITY_TASK_STACK_SIZE, NULL, MEDIUM_PRIORITY_TASK_PRIORITY, &task);

    xTaskCreate(high_priority, "HighPriorityTask",
                PRIORITY_TASK_STACK_SIZE, NULL, HIGH_PRIORITY_TASK_PRIORITY, &task);

    while (true){
        taskYIELD();
    }
}


void main (void)
{   

    semaphore = xSemaphoreCreateBinary();



    stdio_init_all();
    TaskHandle_t task;
    
    vTaskStartScheduler();
    return 0;
}
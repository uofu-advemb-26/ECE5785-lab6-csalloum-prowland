#include "helper.h"

TaskHandle_t start_thread(TaskFunction_t entry, const char *name,
                          UBaseType_t priority, void *params)
{
    TaskHandle_t thread = NULL;
    xTaskCreate(entry, name, THREAD_STACK_SIZE, params, priority, &thread);
    configASSERT(thread != NULL);
    return thread;
}

configRUN_TIME_COUNTER_TYPE thread_runtime(TaskHandle_t thread)
{
    TaskStatus_t status;
    vTaskGetInfo(thread, &status, pdFALSE, eInvalid);
    return status.ulRunTimeCounter;
}

void stop_thread(TaskHandle_t *thread)
{
    if (*thread != NULL) {
        vTaskDelete(*thread);
        *thread = NULL;
    }
}

void busy_busy(__unused void *params)
{
    for (int i = 0; ; i++);
}

void busy_yield(__unused void *params)
{
    for (int i = 0; ; i++) {
        taskYIELD();
    }
}

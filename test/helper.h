#ifndef HELPER_H
#define HELPER_H

#include "FreeRTOS.h"
#include "task.h"

// Supervisor runs the tests, so it sits above every thread it manages.
#define SUPERVISOR_PRIORITY     ( tskIDLE_PRIORITY + 10UL )
#define THREAD_STACK_SIZE       configMINIMAL_STACK_SIZE

#define LOW_PRIORITY            ( tskIDLE_PRIORITY + 1UL )
#define HIGH_PRIORITY           ( tskIDLE_PRIORITY + 2UL )


TaskHandle_t start_thread(TaskFunction_t entry, const char *name,
                          UBaseType_t priority, void *params);

// Total runtime of a thread in microseconds.
configRUN_TIME_COUNTER_TYPE thread_runtime(TaskHandle_t thread);

// Deletes the thread (if not NULL) and clears the handle.
void stop_thread(TaskHandle_t *thread);

// Busy loops from the lab handout, wrapped as task entries.
void busy_busy(void *params);
void busy_yield(void *params);

#endif

#include <stdio.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <unity.h>
#include "unity_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "helper.h"

TaskHandle_t low_thread = NULL;
TaskHandle_t mid_thread = NULL;
TaskHandle_t high_thread = NULL;
SemaphoreHandle_t semaphore = NULL;

void setUp(void) {}

void tearDown(void)
{
    stop_thread(&low_thread);
    stop_thread(&mid_thread);
    stop_thread(&high_thread);
}


// Activity 2: same priority.
void test_same_pri_busy_busy(void)
{
    TEST_IGNORE_MESSAGE("TODO");
}

void test_same_pri_yield_yield(void)
{
    TEST_IGNORE_MESSAGE("TODO");
}

void test_same_pri_busy_yield(void)
{
    TEST_IGNORE_MESSAGE("TODO");
}

// Activity 2: different priority.
void test_diff_pri_busy_busy_high_first(void)
{
    TEST_IGNORE_MESSAGE("TODO");
}

void test_diff_pri_busy_busy_low_first(void)
{
    TEST_IGNORE_MESSAGE("TODO");
}

void test_diff_pri_yield_yield(void)
{
    TEST_IGNORE_MESSAGE("TODO");
}

void supervisor_task(__unused void *params)
{
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000)); // Give time for TTY to attach.
        printf("Start tests\n");
        UNITY_BEGIN();
        RUN_TEST(test_same_pri_busy_busy);
        RUN_TEST(test_same_pri_yield_yield);
        RUN_TEST(test_same_pri_busy_yield);
        RUN_TEST(test_diff_pri_busy_busy_high_first);
        RUN_TEST(test_diff_pri_busy_busy_low_first);
        RUN_TEST(test_diff_pri_yield_yield);
        UNITY_END();
    }
}

int main (void)
{
    stdio_init_all();
    xTaskCreate(supervisor_task, "Supervisor",
                THREAD_STACK_SIZE, NULL, SUPERVISOR_PRIORITY, NULL);
    vTaskStartScheduler();
    return 0;
}

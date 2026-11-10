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

int32_t first = 0;
int32_t second = 0;

void run_pair(TaskFunction_t first, UBaseType_t first_pri,
              TaskFunction_t second, UBaseType_t second_pri,
              TickType_t stagger)
{
    high_thread = start_thread(first, "first", first_pri, NULL);
    vTaskDelay(stagger);
    low_thread = start_thread(second, "second", second_pri, NULL);
    vTaskDelay(pdMS_TO_TICKS(1000));

    first = (int32_t)thread_runtime(high_thread);
    second = (int32_t)thread_runtime(low_thread);
    printf("first %ld us, second %ld us\n", (long)first, (long)second);
}

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
    run_pair(busy_busy, LOW_PRIORITY, busy_busy, LOW_PRIORITY, 0);
    TEST_ASSERT_INT_WITHIN(100000, 500000, first);
    TEST_ASSERT_INT_WITHIN(100000, 500000, second);
}

void test_same_pri_yield_yield(void)
{
    run_pair(busy_yield, LOW_PRIORITY, busy_yield, LOW_PRIORITY, 0);
    TEST_ASSERT_INT_WITHIN(100000, 500000, first);
    TEST_ASSERT_INT_WITHIN(100000, 500000, second);
}

void test_same_pri_busy_yield(void)
{
    run_pair(busy_busy, LOW_PRIORITY, busy_yield, LOW_PRIORITY, 0);
    TEST_ASSERT_INT_WITHIN(100000, 500000, first);
    TEST_ASSERT_INT_WITHIN(100000, 500000, second);
}

// Activity 2: different priority.
void test_diff_pri_busy_busy_high_first(void)
{
    run_pair(busy_busy, HIGH_PRIORITY, busy_busy, LOW_PRIORITY, pdMS_TO_TICKS(100));
    TEST_ASSERT_INT_WITHIN(10000, 100000, first);
    TEST_ASSERT_GREATER_THAN_INT(900000, second);
}

void test_diff_pri_busy_busy_low_first(void)
{
    run_pair(busy_busy, LOW_PRIORITY, busy_busy, HIGH_PRIORITY, pdMS_TO_TICKS(100));
    TEST_ASSERT_INT_WITHIN(10000, 100000, first);
    TEST_ASSERT_GREATER_THAN_INT(900000, second);
}

void test_diff_pri_yield_yield(void)
{
    run_pair(busy_yield, LOW_PRIORITY, busy_yield, HIGH_PRIORITY, pdMS_TO_TICKS(100));
    TEST_ASSERT_INT_WITHIN(10000, 100000, first);
    TEST_ASSERT_GREATER_THAN_INT(900000, second);
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

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"
#include "led_driver.h"

int main(void);
void vApplicationMallocFailedHook (void);
void vApplicationStackOverflowHook(void);
void create_tasks(void);
void Task1(void * pvParameters);
void Task2(void * pvParameters);

int main(void){
    led_init();

    create_tasks();

    vTaskStartScheduler();

    for(;;);

    return 0;
}

void create_tasks(void){
    xTaskCreate(Task1,
                "Task1",
                100,
                NULL,
                2,
                NULL);
    xTaskCreate(Task2,
                "Task2",
                100,
                NULL,
                1,
                NULL);
}

void Task1(void * pvParameters){
    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();
    uint8_t check = 1;
    for(;;){
        if (check){
            led_turn_on(LED_GREEN);
            check = 0;
        }
        else{
            led_turn_off(LED_GREEN);
            check = 1;
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
    }

    vTaskDelete(NULL);
}

void Task2(void* pvParameters){

    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();
    uint8_t check = 1;

    for(;;){
        if(check){
            led_turn_on(LED_RED);
            check = 0;
        }
        else{
            led_turn_off(LED_RED);
            check = 1;
        }
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
    }

    vTaskDelete(NULL);
}

void vApplicationMallocFailedHook(void)
{
}

void vApplicationStackOverflowHook(void)
{
}

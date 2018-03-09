#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

int main(void);
void vApplicationMallocFailedHook (void);
void vApplicationStackOverflowHook(void);
void create_tasks(void);
void Task1(void * pvParameters);
void Task2(void * pvParameters);

int main(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    __asm("dsb");
    GPIOD->MODER |= (1 << (GPIO_PinSource14 * 2));
    GPIOD->MODER |= (1 << (GPIO_PinSource12 * 2));

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
                1,
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

    for(;;){
        GPIOD->ODR ^= (1 << (GPIO_PinSource14));

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
    }

    vTaskDelete(NULL);
}

void Task2(void* pvParameters){

    TickType_t xLastWakeTime;

    xLastWakeTime = xTaskGetTickCount();

    for(;;){
        GPIOD->ODR ^= (1 << (GPIO_PinSource12));

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

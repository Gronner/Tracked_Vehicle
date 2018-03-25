#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"
#include "led_driver.h"
#include "pwm_driver.h"

int main(void);
void vApplicationMallocFailedHook (void);
void vApplicationStackOverflowHook(void);
void create_tasks(void);
void Task1(void * pvParameters);
void Task2(void * pvParameters);

int main(void){
    led_init();
    pwm_init();


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
}

void Task1(void * pvParameters){
    TickType_t xLastWakeTime;
    uint16_t duty_cycle_left = pwm_get_duty_cycle(PWM_LEFT);
    uint16_t duty_cycle_right = pwm_get_duty_cycle(PWM_RIGHT);

    xLastWakeTime = xTaskGetTickCount();
    led_turn_on(LED_GREEN);
    for(;;){
        pwm_set_duty_cycle(PWM_LEFT, duty_cycle_left+20);
        pwm_set_duty_cycle(PWM_RIGHT, duty_cycle_right+10);
        duty_cycle_left = pwm_get_duty_cycle(PWM_LEFT);
        duty_cycle_right = pwm_get_duty_cycle(PWM_RIGHT);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
    }

    vTaskDelete(NULL);
}

void Task2(void* pvParameters){
    TickType_t xLastWakeTime;
    uint8_t led_state;

    xLastWakeTime = xTaskGetTickCount();
    led_turn_on(LED_RED);
    for(;;){
        led_state = led_get_state();
        if(led_state & 0b00000010){
            led_turn_off(LED_RED);
            led_turn_on(LED_BLUE);
        }
        else{
            led_turn_off(LED_BLUE);
            led_turn_on(LED_RED);
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

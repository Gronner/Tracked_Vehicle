#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"
#include "led_driver.h"
#include "pwm_driver.h"
#include "motor_driver.h"

int main(void); void vApplicationMallocFailedHook (void);
void vApplicationStackOverflowHook(void);
void create_tasks(void);
void Task1(void * pvParameters);
void Task2(void * pvParameters);

int main(void){
    led_init();
    // PWM init must always come after motor init!
    motor_init();
    pwm_init();
    
    // Start driving forward
    motor_change_direction(DC_BOTH, DC_DIR_FORWARD);
    pwm_set_duty_cycle(PWM_LEFT, 75);
    pwm_set_duty_cycle(PWM_RIGHT, 75);
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
    xLastWakeTime = xTaskGetTickCount();
    uint8_t i = 0;
    for(;;){
        if(i == 10){
            pwm_set_duty_cycle(PWM_LEFT, 0);
            pwm_set_duty_cycle(PWM_RIGHT, 0);
            motor_change_direction(DC_BOTH, DC_DIR_REVERSE);
            pwm_set_duty_cycle(PWM_LEFT, 75);
            pwm_set_duty_cycle(PWM_RIGHT, 75);
        }
        i++;
        led_toggle(LED_GREEN);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
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

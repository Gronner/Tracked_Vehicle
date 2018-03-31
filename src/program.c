#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"
#include "led_driver.h"
// #include "i2c_driver.h"
#include "lcd_driver.h"

int main(void);
void vApplicationMallocFailedHook (void);
void vApplicationStackOverflowHook(void);
void create_tasks(void);
void Task1(void * pvParameters);
void Task2(void * pvParameters);

int main(void){
    led_init();
    led_turn_on(LED_RED);
    lcd_init();
    led_turn_on(LED_ORANGE);

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
    // uint8_t data_buffer[1] = {LSM_CTR_AXES_ENABLE};
    uint8_t i = 0;
    for(;;){
        led_toggle(LED_GREEN);
        lcd_write_char('a');
        i++;
        if(i > 15){
            lcd_clear();
            i = 0;
        }
        // i2c_write(LSM_ACC_ADR, LSM_CTR_SADR, data_buffer, 1);
        // i2c_read(LSM_ACC_ADR, LSM_ACC_OUT_X_L, data_buffer, 1);
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

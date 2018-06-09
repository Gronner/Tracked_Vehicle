#include <stdint.h>
#include "FreeRTOS.h"
#include "bsp.h"
#include "led_driver.h"
#include "motor_driver.h"
#include "pwm_driver.h"
#include "task.h"

int main(void);
void vApplicationMallocFailedHook(void);
void vApplicationStackOverflowHook(void);
void create_tasks(void);
void Task1(void* pvParameters);
void Task2(void* pvParameters);

int main(void) {
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

  for (;;)
    ;

  return 0;
}

void create_tasks(void) { xTaskCreate(Task1, "Task1", 100, NULL, 2, NULL); }

void Task1(void* pvParameters) {
  TickType_t xLastWakeTime;
  xLastWakeTime = xTaskGetTickCount();
  float volt = 0.f;
  for (;;) {
    motor_set_drive_v(DC_BOTH, volt);
    volt += 0.5f;
    led_toggle(LED_GREEN);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(2000));
  }

  vTaskDelete(NULL);
}

void Task2(void* pvParameters) {
  TickType_t xLastWakeTime;
  uint8_t led_state;

  xLastWakeTime = xTaskGetTickCount();
  led_turn_on(LED_RED);
  for (;;) {
    led_state = led_get_state();
    if (led_state & 0b00000010) {
      led_turn_off(LED_RED);
      led_turn_on(LED_BLUE);
    } else {
      led_turn_off(LED_BLUE);
      led_turn_on(LED_RED);
    }
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(250));
  }

  vTaskDelete(NULL);
}

void vApplicationMallocFailedHook(void) {}

void vApplicationStackOverflowHook(void) {}

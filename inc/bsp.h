#ifndef __BSP_H_
#define __BSP_H_
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"

// LEDs
#define LED_PORT GPIOD
#define LED_GREEN GPIO_Pin_12
#define LED_ORANGE GPIO_Pin_13
#define LED_RED GPIO_Pin_14
#define LED_BLUE GPIO_Pin_15

// PWM
#define PWM_PORT GPIOA
#define PWM_RIGHT GPIO_Pin_1
#define PWM_LEFT GPIO_Pin_15
#define PWM_TIMER TIM2
#define PWM_PIN_AF GPIO_AF_TIM2
#define PWM_PRESCALER 40000
#define PWM_PERIOD 500
#define PWM_CLK_DIV TIM_CKD_DIV1
#define PWM_START_PULSE 0

#endif // __BSP_H_

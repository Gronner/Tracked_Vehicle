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
#define PWM_RIGHT_SRC GPIO_PinSource1
#define PWM_LEFT GPIO_Pin_15
#define PWM_LEFT_SRC GPIO_PinSource15
#define PWM_TIMER TIM2
#define PWM_PIN_AF GPIO_AF_TIM2
#define PWM_PRESCALER 0
#define PWM_PERIOD 8399
#define PWM_CLK_DIV TIM_CKD_DIV1
#define PWM_START_PULSE 0

// I2C
#define I2C_BUS I2C1
#define I2C_PORT GPIOB
#define I2C_SCL GPIO_Pin_6
#define I2C_SCL_SRC GPIO_PinSource6
#define I2C_SDA GPIO_Pin_9
#define I2C_SDA_SRC GPIO_PinSource9
#define I2C_CLOCK_SPEED 100000
#define I2C_OWN_ADDRESS 0x00

// LSM303DLHC
#define LSM_ACC_ADR 0x32
#define LSM_MAG_ADR 0x3C
#define LSM_CTR_ODR_50HZ 0x40
#define LSM_CTR_AXES_ENABLE 0x07
// Register Map
#define LSM_CTR_SADR 0x20
#define LSM_ACC_OUT_X_L 0x28

// LCD
#define LCD_PORT GPIOD
#define LCD_RS GPIO_Pin_6
#define LCD_EN GPIO_Pin_7
#define LCD_D4 GPIO_Pin_8
#define LCD_D5 GPIO_Pin_9
#define LCD_D6 GPIO_Pin_10
#define LCD_D7 GPIO_Pin_11
#define LCD_SHIFT 7
// LCD Command Values
#define LCD_8b 0x03
#define LCD_4b 0x02
#define LCD_DM 0x28
#define LCD_ACT 0x0F
#define LCD_CLR 0x01 // Also resets cursor

#endif // __BSP_H_

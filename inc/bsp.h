#ifndef __BSP_INCLUDED__
#define __BSP_INCLUDED__
/* -------------------------------------------- *
 * This an is automatic generated file.         *
 * Do not alter it, change the specification csv*
 * instead.                                     *
 * -------------------------------------------- */

#include <stm32f4xx_adc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>

// Defines for LED
#define LED_PORT GPIOD
#define LED_GREEN GPIO_Pin_12
#define LED_ORANGE GPIO_Pin_13
#define LED_RED GPIO_Pin_14
#define LED_BLUE GPIO_Pin_15

// Defines for PWM
#define PWM_PORT GPIOA
#define PWM_RIGHT GPIO_Pin_1
#define PWM_RIGHT_SRC GPIO_PinSource1
#define PWM_LEFT GPIO_Pin_15
#define PWM_LEFT_SRC GPIO_PinSource15
#define PWM_TIMER TIM2
#define PWM_PIN_AF GPIO_AF_TIM2
#define PWM_PRESCALER 0
#define PWM_PERIOD 1343
#define PWM_CLK_DIV TIM_CKD_DIV1
#define PWM_START_PULSE 0

// Defines for I2C
#define I2C_BUS I2C1
#define I2C_PORT GPIOB
#define I2C_SCL GPIO_Pin_6
#define I2C_SCL_SRC GPIO_PinSource6
#define I2C_SDA GPIO_Pin_9
#define I2C_SDA_SRC GPIO_PinSource9
#define I2C_CLOCK_SPEED 100000
#define I2C_OWN_ADDRESS 0x00

// Defines for LCD
#define LCD_PORT GPIOD
#define LCD_RS GPIO_Pin_6
#define LCD_EN GPIO_Pin_7
#define LCD_D4 GPIO_Pin_8
#define LCD_D5 GPIO_Pin_9
#define LCD_D6 GPIO_Pin_10
#define LCD_D7 GPIO_Pin_11
#define LCD_SHIFT 8

// Defines for LCD-> CMD
#define LCD_CMD_8b 0x03
#define LCD_CMD_4b 0x02
#define LCD_CMD_DM 0x28
#define LCD_CMD_ACT 0x0F
#define LCD_CMD_CLR 0x01

// Defines for DC-> CTRL
#define DC_CTRL_PORT GPIOB
#define DC_CTRL_PIN_LF GPIO_Pin_12
#define DC_CTRL_PIN_LR GPIO_Pin_13
#define DC_CTRL_PIN_RF GPIO_Pin_14
#define DC_CTRL_PIN_RR GPIO_Pin_15

// Defines for ADC
#define ADC_PORT GPIOB
#define ADC_LEFT_MOTOR GPIO_Pin_0
#define ADC_RIGHT_MOTOR GPIO_Pin_1
#define ADC_LEFT_CHANNEL ADC_Channel_8
#define ADC_RIGHT_CHANNEL ADC_Channel_9
#define ADC_NUMBER_OF_CONV 2
#define ADC_CONV_FACTOR 0.000878f

#endif  //__BSP_INCLUDED__

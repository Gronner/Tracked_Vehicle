#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "led_driver.h"
#include "bsp.h"
#include <stdio.h>

static uint8_t led_status = 0b00000000;

void led_init(void){
    GPIO_InitTypeDef LED_Init_Def;
    LED_Init_Def.GPIO_Pin = LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE;
    LED_Init_Def.GPIO_Mode = GPIO_Mode_OUT;
    LED_Init_Def.GPIO_OType = GPIO_OType_PP;
    LED_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
    LED_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_Init(LED_PORT, &LED_Init_Def);
}

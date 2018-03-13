#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "led_driver.h"
#include "bsp.h"

static uint8_t led_status;

static uint8_t led_get_bitmask(uint16_t led){
    switch(led){
        case LED_GREEN: return 1;
        case LED_ORANGE: return (uint8_t) (1 << 1);
        case LED_RED: return (uint8_t) (1 << 2);
        case LED_BLUE: return (uint8_t) (1 << 3);
        default: return UINT8_MAX;
    }
}
                         

uint8_t led_init(void){
    GPIO_InitTypeDef LED_Init_Def;
    LED_Init_Def.GPIO_Pin = LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE;
    LED_Init_Def.GPIO_Mode = GPIO_Mode_OUT;
    LED_Init_Def.GPIO_OType = GPIO_OType_PP;
    LED_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
    LED_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    GPIO_Init(LED_PORT, &LED_Init_Def);

    GPIO_ResetBits(LED_PORT, LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE);
    led_status = 0;

    return led_status;
}

uint8_t led_turn_on(uint16_t led){
    GPIO_SetBits(GPIOD, led);
    led_status |= led_get_bitmask(led);
    return led_status;
}

uint8_t led_turn_off(uint16_t led){
    GPIO_ResetBits(GPIOD, led);
    led_status &= ~(led_get_bitmask(led));
    return led_status;
}

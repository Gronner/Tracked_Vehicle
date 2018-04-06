#include "motor_driver.h"
#include "bsp.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

void motor_init(void){
    // Activate clock to GPIO peripheral
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    
    // Configure control pins
    GPIO_InitTypeDef Ctrl_Init_Def;
    uint16_t all_ctrl_pins = DC_CTRL_PIN_LF | DC_CTRL_PIN_LR | DC_CTRL_PIN_RF | DC_CTRL_PIN_RR;

    Ctrl_Init_Def.GPIO_Pin = all_ctrl_pins;
    Ctrl_Init_Def.GPIO_Mode = GPIO_Mode_OUT;
    Ctrl_Init_Def.GPIO_OType = GPIO_OType_PP;
    Ctrl_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
    Ctrl_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(DC_CTRL_PORT, &Ctrl_Init_Def);

    // Reset pins to avoid damage to motor controller
    GPIO_ResetBits(DC_CTRL_PORT, all_ctrl_pins);
    
}

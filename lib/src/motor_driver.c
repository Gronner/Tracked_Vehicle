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

void motor_change_direction(uint8_t side, uint8_t direction){
    uint16_t new_active_pin, old_active_pin;
    if(side == DC_LEFT){
        if(direction == DC_DIR_REVERSE){
            new_active_pin = DC_CTRL_PIN_LR;
            old_active_pin = DC_CTRL_PIN_LF;
        }
        if(direction == DC_DIR_FORWARD){
            new_active_pin = DC_CTRL_PIN_LF;
            old_active_pin = DC_CTRL_PIN_LR;
        }
    }
    if(side == DC_RIGHT){
        if(direction == DC_DIR_REVERSE){
            new_active_pin = DC_CTRL_PIN_RR;
            old_active_pin = DC_CTRL_PIN_RF;
        }
        if(direction == DC_DIR_FORWARD){
            new_active_pin = DC_CTRL_PIN_RF;
            old_active_pin = DC_CTRL_PIN_RR;
        }
    }
    if(side == DC_BOTH){
        if(direction == DC_DIR_REVERSE){
            new_active_pin = DC_CTRL_PIN_LR | DC_CTRL_PIN_RR;
            old_active_pin = DC_CTRL_PIN_LF | DC_CTRL_PIN_RF;
        }
        if(direction == DC_DIR_FORWARD){
            new_active_pin = DC_CTRL_PIN_LF | DC_CTRL_PIN_RF;
            old_active_pin = DC_CTRL_PIN_LR | DC_CTRL_PIN_RR;
        }
    }
    if((GPIO_ReadOutputData(DC_CTRL_PORT) & old_active_pin)){
        GPIO_ResetBits(DC_CTRL_PORT, old_active_pin);
        // Wait until reversing is safe
        uint16_t counter = 5971 * DC_SAFE_TIME; // 5971 is around 1ms on the STM32F411
        while(counter -- > 0);
    }
    GPIO_SetBits(DC_CTRL_PORT, new_active_pin);

}

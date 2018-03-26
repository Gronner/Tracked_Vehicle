#include "i2c_driver.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"

#include "bsp.h"

void i2c_init(void){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // Configure Pins
    GPIO_InitTypeDef I2C_Pin_Init_Def;
    I2C_Pin_Init_Def.GPIO_Pin = I2C_SCL | I2C_SDA;
    I2C_Pin_Init_Def.GPIO_Mode = GPIO_Mode_AF;
    I2C_Pin_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
    I2C_Pin_Init_Def.GPIO_OType = GPIO_OType_OD;
    I2C_Pin_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_Init(I2C_PORT, &I2C_Pin_Init_Def);

    GPIO_PinAFConfig(I2C_PORT, I2C_SDA_SRC, GPIO_AF_I2C1);
    GPIO_PinAFConfig(I2C_PORT, I2C_SCL_SRC, GPIO_AF_I2C1);

    // Configure I2C
    I2C_InitTypeDef I2C_Init_Def;
    I2C_Init_Def.I2C_ClockSpeed = I2C_CLOCK_SPEED;
    I2C_Init_Def.I2C_Mode = I2C_Mode_I2C;
    I2C_Init_Def.I2C_DutyCycle = I2C_DutyCycle_16_9;
    I2C_Init_Def.I2C_OwnAddress1 = I2C_OWN_ADRESS;
    I2C_Init_Def.I2C_Ack = I2C_Ack_Enable;
    I2C_Init_Def.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2C_BUS, &I2C_Init_Def);
    I2C_Cmd(I2C_BUS, ENABLE);
}

#include "i2c_driver.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_i2c.h"

#include "bsp.h"

void i2c_init(void){
    // Enable peripheral clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // Configure Pins
    GPIO_PinAFConfig(I2C_PORT, I2C_SDA_SRC, GPIO_AF_I2C1);
    GPIO_PinAFConfig(I2C_PORT, I2C_SCL_SRC, GPIO_AF_I2C1);

    GPIO_InitTypeDef I2C_Pin_Init_Def;
    I2C_Pin_Init_Def.GPIO_Pin = I2C_SCL | I2C_SDA;
    I2C_Pin_Init_Def.GPIO_Mode = GPIO_Mode_AF;
    I2C_Pin_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
    I2C_Pin_Init_Def.GPIO_OType = GPIO_OType_PP;
    I2C_Pin_Init_Def.GPIO_PuPd = GPIO_PuPd_DOWN;

    GPIO_Init(I2C_PORT, &I2C_Pin_Init_Def);

    // Configure I2C
    I2C_InitTypeDef I2C_Init_Def;
    I2C_Init_Def.I2C_ClockSpeed = I2C_CLOCK_SPEED;
    I2C_Init_Def.I2C_Mode = I2C_Mode_I2C;
    I2C_Init_Def.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_Init_Def.I2C_OwnAddress1 = I2C_OWN_ADDRESS;
    I2C_Init_Def.I2C_Ack = I2C_Ack_Enable;
    I2C_Init_Def.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Init(I2C_BUS, &I2C_Init_Def);
    I2C_Cmd(I2C_BUS, ENABLE);
}

void i2c_write(uint8_t address, uint8_t subaddress, uint8_t* data_buffer, uint8_t buffer_len){
    // Send Start signal
    I2C_GenerateSTART(I2C_BUS, ENABLE);
    // Wait till master mode is engaged
    while(!I2C_CheckEvent(I2C_BUS, I2C_EVENT_MASTER_MODE_SELECT));
    // Send slave address
    I2C_Send7bitAddress(I2C_BUS, address, I2C_Direction_Transmitter);
    // Wait till master transmitter mode is engaged
    while(!I2C_CheckEvent(I2C_BUS, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    // Send register subaddress
    I2C_SendData(I2C_BUS, subaddress);
    // Wait for byte to be transmitted
    while(!I2C_CheckEvent(I2C_BUS, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    uint8_t i;
    for(i = 0; i < buffer_len; i++)
    {
        I2C_SendData(I2C_BUS, data_buffer[i]);
        while(!I2C_CheckEvent(I2C_BUS, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
    // Send Stop signal
    I2C_GenerateSTOP(I2C_BUS, ENABLE);
}

void i2c_read(uint8_t address, uint8_t subaddress, uint8_t* data_buffer, uint8_t buffer_len){
    // Send Start Signal
    I2C_GenerateSTART(I2C_BUS, ENABLE);
    // Wait till master mode is engaged
    while(!I2C_CheckEvent(I2C_BUS, I2C_EVENT_MASTER_MODE_SELECT));
    // Send slave addess in transmitter mode
    I2C_Send7bitAddress(I2C_BUS, address, I2C_Direction_Transmitter);
    // Wait till transmitter mode is engaged
    while(!I2C_CheckEvent(I2C_BUS, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    // Send register subaddress
    I2C_SendData(I2C_BUS, subaddress);
    // Wait for byte to be transmitted
    while(!I2C_CheckEvent(I2C_BUS, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send Start Signal
    I2C_GenerateSTART(I2C_BUS, ENABLE);
    // Wait till master mode is engaged
    while(!I2C_CheckEvent(I2C_BUS, I2C_EVENT_MASTER_MODE_SELECT));
    // Send slave addess in receiver mode
    I2C_Send7bitAddress(I2C_BUS, address, I2C_Direction_Receiver);
    // Wait till ADDR is set
    while(!I2C_GetFlagStatus(I2C_BUS, I2C_FLAG_ADDR));
    // Clear ACK
    I2C_AcknowledgeConfig(I2C_BUS, DISABLE);
#ifndef TESTING
    __disable_irq();
    // Clear ADDR
    volatile uint32_t temp __attribute__((unused)); // Used only for clearing Bit!
    temp = I2C_BUS->SR2;
#endif // TESTING
    // Send Stop Signal
    I2C_GenerateSTOP(I2C_BUS, ENABLE);
#ifndef TESTING
    __enable_irq();
    // Poll RxNE
    while((I2C_GetLastEvent(I2C_BUS) & 0x0040) != 0x000040);
#endif // TESTING
    // Read Data
    data_buffer[0] = I2C_ReceiveData(I2C_BUS);
    // Wait for STOP bit to be cleared
#ifndef TESTING
    while((I2C_BUS->CR1 & 0x200) == 0x200);
#endif // TESTING
    // Enable ACK
    I2C_AcknowledgeConfig(I2C_BUS, ENABLE);
}

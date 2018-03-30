#ifndef __TEST_I2C_MOCK_H
#define __TEST_I2C_MOCK_H

extern "C"{
    #include "stm32f4xx_i2c.h"
}

int i2c_is_equal(const void* object1, const void* object2);
const char* i2c_to_string(const void* object);

void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_Init_Struct);
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState);

void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState);
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction);
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data);
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);

#endif // __TEST_I2C_MOCK_H

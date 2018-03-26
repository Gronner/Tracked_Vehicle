#ifndef __TEST_I2C_MOCK_H
#define __TEST_I2C_MOCK_H

extern "C"{
    #include "stm32f4xx_i2c.h"
}

int i2c_is_equal(const void* object1, const void* object2);
const char* i2c_to_string(const void* object);

void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_Init_Struct);
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState);

#endif // __TEST_I2C_MOCK_H

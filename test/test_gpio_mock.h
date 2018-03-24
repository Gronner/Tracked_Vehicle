#ifndef __TEST_GPIO_MOCK_H_
#define __TEST_GPIO_MOCK_H_

extern "C"{
    #include "stm32f4xx_gpio.h"
}

int gpio_is_equal(const void* object1, const void* object2);
const char* gpio_to_string(const void* object);

void GPIO_Init(GPIO_TypeDef* Port, GPIO_InitTypeDef* Pin_Init_Struct);
void GPIO_ResetBits(GPIO_TypeDef* Port, uint16_t GPIO_Pin);
void GPIO_SetBits(GPIO_TypeDef* Port, uint16_t GPIO_Pin);
void GPIO_Bits(GPIO_TypeDef* Port, uint16_t GPIO_Pin);
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF);
#endif // __TEST_GPIO_MOCK_H_

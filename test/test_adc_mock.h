#ifndef __TEST_ADC_MOCK_H_
#define __TEST_ADC_MOCK_H_

extern "C"{
    #include "stm32f4xx_adc.h"
}

int adc_is_equal(const void* object1, const void* object2);
const char* adc_to_string(const void* object);

void ADC_Init(GPIO_TypeDef* Port, GPIO_InitTypeDef* Pin_Init_Struct);
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState);
#endif // __TEST_GPIO_MOCK_H_

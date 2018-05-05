#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_adc.h"
#include "adc_driver.h"
#include "bsp.h"

void adc_init(void){
    // Activate Clocks to GPIO Port and ADC
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    // Configure ADC Pins
    GPIO_InitTypeDef ADC_Pin_Init_Def;
    ADC_Pin_Init_Def.GPIO_Pin = ADC_LEFT_MOTOR | ADC_RIGHT_MOTOR;
    ADC_Pin_Init_Def.GPIO_Mode = GPIO_Mode_AN;
    ADC_Pin_Init_Def.GPIO_OType = GPIO_OType_PP;
    ADC_Pin_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
    ADC_Pin_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(ADC_PORT, &ADC_Pin_Init_Def);

    // Configure ADC
    ADC_InitTypeDef ADC_Init_Def;
    ADC_Init_Def.ADC_Resolution = ADC_Resolution_12b;
    ADC_Init_Def.ADC_ScanConvMode = DISABLE;
    ADC_Init_Def.ADC_ContinuousConvMode = DISABLE;
    ADC_Init_Def.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_Init_Def.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_Init_Def.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_Init_Def.ADC_NbrOfConversion = ADC_NUMBER_OF_CONV;

    ADC_Init(ADC1, &ADC_Init_Def);
    
    // Configure ADC Channels
    ADC_RegularChannelConfig(ADC1, ADC_LEFT_CHANNEL, 1, ADC_SampleTime_3Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_RIGHT_CHANNEL, 2, ADC_SampleTime_3Cycles);

    // Activate ADC
    ADC_Cmd(ADC1, ENABLE);

}

void adc_sample(uint16_t data[]){
    ADC_SoftwareStartConv(ADC1);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    data[0] = ADC_GetConversionValue(ADC1);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    data[1] = ADC_GetConversionValue(ADC1);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
}

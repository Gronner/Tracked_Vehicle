#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "stm32f4xx_adc.h"
    #include "bsp.h"
    #include "adc_driver.h"
}

#include "test_rcc_mock.h"
#include "test_gpio_mock.h"
#include "test_adc_mock.h"

GPIO_InitTypeDef ADC_Pin_Init_Def;
ADC_InitTypeDef ADC_Init_Def;

TEST_GROUP(AdcDriverTestGroup){
    void setup(void){
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
        ADC_Pin_Init_Def.GPIO_Pin = ADC_LEFT_MOTOR | ADC_RIGHT_MOTOR;
        ADC_Pin_Init_Def.GPIO_Mode = GPIO_Mode_AN;
        ADC_Pin_Init_Def.GPIO_OType = GPIO_OType_PP;
        ADC_Pin_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
        ADC_Pin_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;

        ADC_Init_Def.ADC_Resolution = ADC_Resolution_12b;
        ADC_Init_Def.ADC_ScanConvMode = DISABLE;
        ADC_Init_Def.ADC_ContinuousConvMode = DISABLE;
        ADC_Init_Def.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
        ADC_Init_Def.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
        ADC_Init_Def.ADC_DataAlign = ADC_DataAlign_Right;
        ADC_Init_Def.ADC_NbrOfConversion = ADC_NUMBER_OF_CONV;
    }

    void teardown(void){
        mock().clear();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

/* For a passing test the folloing has to happen:
 * Enable peripheral clock to timer and GPIOs
 * Call Timer Init with proper settings
 * Call PWM Init twice with proper settings
 * Call GPIO Init with proper settings
 */
TEST(AdcDriverTestGroup, ADCInitProperly){
    mock_c()->installComparator("GPIO_InitType", gpio_is_equal, gpio_to_string);
    mock_c()->installComparator("ADC_InitType", adc_is_equal, adc_to_string);
    // Enable Clocks for Port and ADC
    mock().expectOneCall("RCC_AHB1PeriphClockCmd").withParameter("HW_Clock", RCC_AHB1Periph_GPIOB)
                                                  .withParameter("STATE", ENABLE);
    mock().expectOneCall("RCC_APB2PeriphClockCmd").withParameter("HW_Clock", RCC_APB2Periph_ADC1)
                                                  .withParameter("STATE", ENABLE);
    // Initalize ADC Pins
    mock().expectOneCall("GPIO_Init").withParameter("Port", ADC_PORT)
                                     .withParameterOfType("GPIO_InitType",
                                                          "Pin_Init_Struct",
                                                          &ADC_Pin_Init_Def);
    // Initalize the ADC
    mock().expectOneCall("ADC_Init").withParameter("ADC", ADC1)
                                    .withParameterOfType("ADC_InitType",
                                                         "ADC_Init_Struct",
                                                         &ADC_Init_Def);
    // Initalize Channels 8 and 9
    mock().expectOneCall("ADC_RegularChannelConfig").withParameter("ADC", ADC1)
                                                    .withParameter("Channel", ADC_LEFT_CHANNEL)
                                                    .withParameter("Rank", 1)
                                                    .withParameter("SampleTime", ADC_SampleTime_3Cycles); 

    mock().expectOneCall("ADC_RegularChannelConfig").withParameter("ADC", ADC1)
                                                    .withParameter("Channel", ADC_RIGHT_CHANNEL)
                                                    .withParameter("Rank", 2)
                                                    .withParameter("SampleTime", ADC_SampleTime_3Cycles); 

    // Activate ADC
    mock().expectOneCall("ADC_Cmd").withParameter("ADC", ADC1)
                                   .withParameter("STATE", ENABLE);
    
    adc_init();
    mock().checkExpectations();
}

TEST(AdcDriverTestGroup, ADCSampleOnce){
    // Start conversion
    mock().expectOneCall("ADC_SoftwareStartConv").withParameter("ADC", ADC1);
    // Wait for conversion to finish
    mock().expectOneCall("ADC_GetFlagStatus").withParameter("ADC", ADC1)
                                             .withParameter("Flag", ADC_FLAG_EOC);
    // Get first Value
    mock().expectOneCall("ADC_GetConversionValue").withParameter("ADC", ADC1);
    // Clear EOC flag
    mock().expectOneCall("ADC_ClearFlag").withParameter("ADC", ADC1)
                                         .withParameter("Flag", ADC_FLAG_EOC);
    // Wait for second conversion to finish
    mock().expectOneCall("ADC_GetFlagStatus").withParameter("ADC", ADC1)
                                             .withParameter("Flag", ADC_FLAG_EOC);
    // Get second Value
    mock().expectOneCall("ADC_GetConversionValue").withParameter("ADC", ADC1);
    // Clear EOC flag
    mock().expectOneCall("ADC_ClearFlag").withParameter("ADC", ADC1)
                                         .withParameter("Flag", ADC_FLAG_EOC);

    uint16_t data[2] = {0, 0};
    adc_sample(data);
    mock().checkExpectations();
    CHECK(data[0] == 2048);
    CHECK(data[1] == 2048);
}

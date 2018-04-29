#include "MockSupport.h"
#include "MockSupport_c.h"
#include "test_adc_mock.h"

extern "C"{
    #include "stdio.h"
}

// Comperator functions
// ADC_InitTypeDef:
int adc_is_equal(const void* object1, const void* object2){
    ADC_InitTypeDef* init1 = (ADC_InitTypeDef*) object1;
    ADC_InitTypeDef* init2 = (ADC_InitTypeDef*) object2;

    if(init1->ADC_Resolution != init2->ADC_Resolution){
        return 0;
    }

    if(init1->ADC_ScanConvMode != init2->ADC_ScanConvMode){
        return 0;
    }

    if(init1->ADC_ContinuousConvMode != init2->ADC_ContinuousConvMode){
        return 0;
    }

    if(init1->ADC_ExternalTrigConvEdge != init2->ADC_ExternalTrigConvEdge){
        return 0;
    }

    if(init1->ADC_ExternalTrigConv != init2->ADC_ExternalTrigConv){
        return 0;
    }

    if(init1->ADC_DataAlign != init2->ADC_DataAlign){
        return 0;
    }

    if(init1->ADC_NbrOfConversion != init2->ADC_NbrOfConversion){
        return 0;
    }

    return 1;
}

const char* adc_to_string(const void* object){
    char str[9];
    ADC_InitTypeDef* init = (ADC_InitTypeDef*) object;
    sprintf(str, "%X", init->ADC_Resolution);
    const char* str_const = str;
    return str_const;
}

// GPIO mock functions
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_Init_Struct){
    mock().actualCall("ADC_Init").withParameter("ADC", ADCx)
                                 .withParameterOfType("ADC_InitType", 
                                                      "ADC_Init_Struct", 
                                                      ADC_Init_Struct);
}

void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime){
    mock().actualCall("ADC_RegularChannelConfig").withParameter("ADC", ADCx)
                                                 .withParameter("Channel", ADC_Channel)
                                                 .withParameter("Rank", Rank)
                                                 .withParameter("SampleTime", ADC_SampleTime);
}

void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState){
    mock().actualCall("ADC_Cmd").withParameter("ADC", ADCx)
                                .withParameter("STATE", NewState);
}

#include "MockSupport.h"
#include "MockSupport_c.h"
#include "test_gpio_mock.h"

extern "C"{
    #include "stdio.h"
}

// Comperator functions
// GPIO_InitTypeDef:
int gpio_is_equal(const void* object1, const void* object2){
    GPIO_InitTypeDef* init1 = (GPIO_InitTypeDef*) object1;
    GPIO_InitTypeDef* init2 = (GPIO_InitTypeDef*) object2;

    if (init1->GPIO_Pin != init2->GPIO_Pin){
        return 0;
    }
    if (init1->GPIO_Mode != init2->GPIO_Mode){
        return 0;
    }
    if (init1->GPIO_OType != init2->GPIO_OType){
        return 0;
    }
    if (init1->GPIO_PuPd != init2->GPIO_PuPd){
        return 0;
    }
    if (init1->GPIO_Speed != init2->GPIO_Speed){
        return 0;
    }
    return 1;
}

const char* gpio_to_string(const void* object){
    char str[9];
    GPIO_InitTypeDef* init = (GPIO_InitTypeDef*) object;
    sprintf(str, "%X", init->GPIO_Pin);
    const char* str_const = str;
    return str_const;
}

// GPIO mock functions
void GPIO_Init(GPIO_TypeDef* Port, GPIO_InitTypeDef* Pin_Init_Struct){
    mock().actualCall("GPIO_Init").withParameter("Port", Port)
                                  .withParameterOfType("GPIO_InitType", "Pin_Init_Struct", Pin_Init_Struct);
}

void GPIO_ResetBits(GPIO_TypeDef* Port, uint16_t GPIO_Pin){
    mock().actualCall("GPIO_ResetBits").withParameter("Port", Port)
                                       .withParameter("GPIO_Pin", GPIO_Pin);
}

void GPIO_SetBits(GPIO_TypeDef* Port, uint16_t GPIO_Pin){
    mock().actualCall("GPIO_SetBits").withParameter("Port", Port)
                                     .withParameter("GPIO_Pin", GPIO_Pin);
}

void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF){
    mock().actualCall("GPIO_PinAFConfig").withParameter("Port", GPIOx)
                                         .withParameter("Pins", GPIO_PinSource) 
                                         .withParameter("AF_Sel", GPIO_AF);
}

void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal){
    mock().actualCall("GPIO_Write").withParameter("Port", GPIOx)
                                   .withParameter("Port_Value", PortVal);
}

uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx){
    mock().actualCall("GPIO_ReadOutputData").withParameter("Port", GPIOx);
    return 0x9; // Both motors driving forward
}

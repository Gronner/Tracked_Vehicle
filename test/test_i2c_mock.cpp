#include "MockSupport.h"
#include "MockSupport_c.h"
#include "test_i2c_mock.h"

extern "C"{
    #include "stdio.h"
}

int i2c_is_equal(const void* object1, const void* object2){
    I2C_InitTypeDef* init1 = (I2C_InitTypeDef*) object1;
    I2C_InitTypeDef* init2 = (I2C_InitTypeDef*) object2;

    if(init1->I2C_ClockSpeed != init2->I2C_ClockSpeed)
    {
        return 0;
    }
    if(init1->I2C_Mode != init2->I2C_Mode)
    {
        return 0;
    }
    if(init1->I2C_DutyCycle != init2->I2C_DutyCycle)
    {
        return 0;
    }
    if(init1->I2C_OwnAddress1 != init2->I2C_OwnAddress1)
    {
        return 0;
    }
    if(init1->I2C_Ack != init2->I2C_Ack)
    {
        return 0;
    }
    if(init1->I2C_AcknowledgedAddress != init2->I2C_AcknowledgedAddress)
    {
        return 0;
    }
    return 1;
}

const char* i2c_to_string(const void* object){
    char str[9];
    I2C_InitTypeDef* init = (I2C_InitTypeDef*) object;
    sprintf(str, "%d", init->I2C_ClockSpeed); 
    const char* str_const = str;
    return str_const;
}

void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_Init_Struct){
    mock().actualCall("I2C_Init").withParameter("I2C", I2Cx)
                                 .withParameterOfType("I2C_InitType",
                                                      "I2C_Init_Struct",
                                                      I2C_Init_Struct);
}

void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState){
    mock().actualCall("I2C_Cmd").withParameter("I2C", I2Cx)
                                .withParameter("STATE", NewState);
}

void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState){
    mock().actualCall("I2C_GenerateSTART").withParameter("I2C", I2Cx)
                                          .withParameter("STATE", NewState);
}
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState){
    mock().actualCall("I2C_GenerateSTOP").withParameter("I2C", I2Cx)
                                          .withParameter("STATE", NewState);
}

void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction){
    mock().actualCall("I2C_Send7bitAddress").withParameter("I2C", I2Cx)
                                            .withParameter("Address", Address)
                                            .withParameter("Direction", I2C_Direction);
}

void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data){
    mock().actualCall("I2C_SendData").withParameter("I2C", I2Cx)
                                     .withParameter("Data", Data);
}

ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT){
    mock().actualCall("I2C_CheckEvent").withParameter("I2C", I2Cx)
                                       .withParameter("Event", I2C_EVENT);
    return SUCCESS;
}

void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState){
    mock().actualCall("I2C_AcknowledgeConfig").withParameter("I2C", I2Cx)
                                              .withParameter("STATE", NewState);
}

uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx){
    mock().actualCall("I2C_ReceiveData").withParameter("I2C", I2Cx);
    return 0xAA;
}

FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG){
    mock().actualCall("I2C_GetFlagStatus").withParameter("I2C", I2Cx)
                                          .withParameter("Flag", I2C_FLAG);
    return SET;
}


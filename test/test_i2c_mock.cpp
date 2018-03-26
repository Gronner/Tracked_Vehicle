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

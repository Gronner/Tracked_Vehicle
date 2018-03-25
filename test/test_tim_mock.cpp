#include "MockSupport.h"
#include "MockSupport_c.h"
#include "test_tim_mock.h"

extern "C"{
    #include "stdio.h"
}

// Comperator functions
// TIM_TimeBaseInitTypeDef
int tim_is_equal(const void* object1, const void* object2){
    TIM_TimeBaseInitTypeDef* init1 = (TIM_TimeBaseInitTypeDef*) object1;
    TIM_TimeBaseInitTypeDef* init2 = (TIM_TimeBaseInitTypeDef*) object2;

    if(init1->TIM_Prescaler != init2->TIM_Prescaler){
        return 0;
    }
    if(init1->TIM_CounterMode != init2->TIM_CounterMode){
        return 0;
    }
    if(init1->TIM_Period != init2->TIM_Period){
        return 0;
    }
    if(init1->TIM_ClockDivision != init2->TIM_ClockDivision){
        return 0;
    }
    if(init1->TIM_RepetitionCounter != init2->TIM_RepetitionCounter){
        return 0;
    }
    return 1;
}

const char* tim_to_string(const void* object){
    char str[17];
    TIM_TimeBaseInitTypeDef* init = (TIM_TimeBaseInitTypeDef*) object;
    sprintf(str, "%d", init->TIM_Prescaler);
    const char* str_const = str;
    return str_const;
}

//TIM_OCInitTypeDef
int tim_oc_is_equal(const void* object1, const void* object2){
    TIM_OCInitTypeDef* init1 = (TIM_OCInitTypeDef*) object1;
    TIM_OCInitTypeDef* init2 = (TIM_OCInitTypeDef*) object2;

    if(init1->TIM_OCMode != init2->TIM_OCMode){
        return 0;
    }
    if(init1->TIM_OutputState != init2->TIM_OutputState){
        return 0;
    }
    if(init1->TIM_Pulse != init2->TIM_Pulse){
        return 0;
    }
    if(init1->TIM_OCPolarity != init2->TIM_OCPolarity){
        return 0;
    }
    return 1;
}

const char* tim_oc_to_string(const void* object){
    char str[17];
    TIM_OCInitTypeDef* init = (TIM_OCInitTypeDef*) object;
    sprintf(str, "%d", init->TIM_Pulse);
    const char* str_const = str;
    return str_const;
}

void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct){
    mock().actualCall("TIM_TimeBaseInit").withParameter("Timer", TIMx)
                                         .withParameterOfType("TIM_InitType", 
                                                              "TIM_Init_Struct",
                                                              TIM_TimeBaseInitStruct);
}
                                         
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState){
    mock().actualCall("TIM_Cmd").withParameter("Timer", TIMx)
                                .withParameter("STATE", NewState);
}

void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct){
    mock().actualCall("TIM_OC1Init").withParameter("Timer", TIMx)
                                    .withParameterOfType("TIM_CH_InitType",
                                                         "TIM_CH_Init_Struct",
                                                         TIM_OCInitStruct);
}

void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload){
    mock().actualCall("TIM_OC1PreloadConfig").withParameter("Timer", TIMx)
                                             .withParameter("State", TIM_OCPreload);
}

void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct){
    mock().actualCall("TIM_OC2Init").withParameter("Timer", TIMx)
                                    .withParameterOfType("TIM_CH_InitType",
                                                         "TIM_CH_Init_Struct",
                                                         TIM_OCInitStruct);
}

void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload){
    mock().actualCall("TIM_OC2PreloadConfig").withParameter("Timer", TIMx)
                                             .withParameter("State", TIM_OCPreload);
}

void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare){
    mock().actualCall("TIM_SetCompare1").withParameter("Timer", TIMx)
                                        .withParameter("CCR", Compare);
}

void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare){
    mock().actualCall("TIM_SetCompare2").withParameter("Timer", TIMx)
                                        .withParameter("CCR", Compare);
}

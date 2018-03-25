#ifndef __TEST_TIM_MOCK_H_
#define __TEST_TIM_MOCK_H_
extern "C"{
    #include "stm32f4xx_tim.h"
}

int tim_is_equal(const void* object1, const void* object2);
const char* tim_to_string(const void* object);

int tim_oc_is_equal(const void* object1, const void* object2);
const char* tim_oc_to_string(const void* object);

void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct);
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload);
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare);
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint32_t Compare);
#endif // __TEST_TIM_MOCK_H_


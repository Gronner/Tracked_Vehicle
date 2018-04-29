#ifndef __TEST_RCC_MOCK_H_
#define __TEST_RCC_MOCK_H_
extern "C"{
    #include "stm32f4xx_rcc.h"
}

void RCC_AHB1PeriphClockCmd(uint32_t HW_Clock, FunctionalState State);
void RCC_APB1PeriphClockCmd(uint32_t HW_Clock, FunctionalState State);
void RCC_APB2PeriphClockCmd(uint32_t HW_Clock, FunctionalState State);

#endif // __TEST_RCC_MOCK_H_

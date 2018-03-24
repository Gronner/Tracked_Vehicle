#include "MockSupport.h"
#include "MockSupport_c.h"
#include "test_rcc_mock.h"

void RCC_AHB1PeriphClockCmd(uint32_t HW_Clock, FunctionalState State){
    mock().actualCall("RCC_AHB1PeriphClockCmd").withParameter("HW_Clock", HW_Clock)
                                                              .withParameter("STATE", State);
}

void RCC_APB1PeriphClockCmd(uint32_t HW_Clock, FunctionalState State){
    mock().actualCall("RCC_APB1PeriphClockCmd").withParameter("HW_Clock", HW_Clock)
                                                              .withParameter("STATE", State);
}

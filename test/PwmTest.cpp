#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "bsp.h"
    #include "pwd_driver.h"
}

#include "test_rcc_mock.h"
#include "test_gpio_mock.h"
#include "test_tim_mock.h"

TIM_TimeBaseInitTypeDef TIM2_Init_Def;
TIM_OCInitTypeDef TIM2_CH_Init_Def;
GPIO_InitTypeDef PWM_Init_Def;

TEST_GROUP(PwdDriverTestGroup){
    void setup(void){
        // Setup TIM 2
        TIM2_Init_Def.TIM_Prescaler = PWM_PRESCALER;
        TIM2_Init_Def.TIM_CounterMode = TIM_CounterMode_Up; // Count up
        TIM2_Init_Def.TIM_Period = PWM_PERIOD;
        TIM2_Init_Def.TIM_ClockDivision = PWM_CLK_DIV;
        TIM2_Init_Def.TIM_RepetitionCounter = 0;            // No Repetitions
        // Setup TIM 2 channel output as pwm
        TIM2_CH_Init_Def.TIM_OCMode = TIM_OCMode_PWM1;
        TIM2_CH_Init_Def.TIM_Pulse = PWM_START_PULSE;
        TIM2_CH_Init_Def.TIM_OutputState = TIM_OutputState_Enable;
        TIM2_CH_Init_Def.TIM_OCPolarity = TIM_OCPolarity_High;
        // Setup Pin A1 and A15 as alternative function output
        PWM_Init_Def.GPIO_Pin = PWM_RIGHT | PWM_LEFT;
        PWM_Init_Def.GPIO_Mode = GPIO_Mode_AF;
        PWM_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
    }

    void teardown(void){
    }
};

/* For a passing test the folloing has to happen:
 * Enable peripheral clock to timer and GPIOs
 * Call Timer Init with proper settings
 * Call PWM Init twice with proper settings
 * Call GPIO Init with proper settings
 */
TEST(PwdDriverTestGroup, PWMInitProperly){
    mock_c()->installComparator("GPIO_InitType", gpio_is_equal, gpio_to_string);
    mock_c()->installComparator("TIM_InitType", tim_is_equal, tim_to_string);
    mock_c()->installComparator("TIM_CH_InitType", tim_oc_is_equal, tim_oc_to_string);

    // Check for clocks
    mock().expectOneCall("RCC_APB1PeriphClockCmd").withParameter("HW_Clock", RCC_APB1Periph_TIM2)
                                                  .withParameter("STATE", ENABLE);
    mock().expectOneCall("RCC_AHB1PeriphClockCmd").withParameter("HW_Clock", RCC_AHB1Periph_GPIOA)
                                                  .withParameter("STATE", ENABLE);
    // Check for timer
    mock().expectOneCall("TIM_TimeBaseInit").withParameter("Timer", PWM_TIMER)
                                            .withParameterOfType("TIM_InitType",
                                                                 "TIM_Init_Struct",
                                                                 &TIM2_Init_Def);
    mock().expectOneCall("TIM_Cmd").withParameter("Timer", PWM_TIMER)
                                   .withParameter("STATE", ENABLE);
    // Check for PWM
    mock().expectOneCall("TIM_OC1Init").withParameter("Timer", PWM_TIMER)
                                       .withParameterOfType("TIM_CH_InitType",
                                                            "TIM_CH_Init_Struct",
                                                            &TIM2_CH_Init_Def);
    mock().expectOneCall("TIM_OC1PreloadConfig").withParameter("Timer", PWM_TIMER)
                                                .withParameter("State", TIM_OCPreload_Enable);
    // Check for GPIO
    mock().expectOneCall("GPIO_PinAFConfig").withParameter("Port", PWM_PORT)
                                            .withParameter("Pins", PWM_RIGHT | PWM_LEFT)
                                            .withParameter("AF_Sel", PWM_PIN_AF);
    mock().expectOneCall("GPIO_Init").withParameter("Port", LED_PORT)
                                     .withParameterOfType("GPIO_InitType",
                                                          "Pin_Init_Struct",
                                                          &PWM_Init_Def);
    //pwm_init();
    mock().checkExpectations();
}

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "bsp.h"
#include "pwm_driver.h"
#include "led_driver.h"

void pwm_init(void){
    // Activate clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // PWM timer clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);// GPIO Port A timer clock
    // Configure timer
    TIM_TimeBaseInitTypeDef TIM2_Init_Def;
    TIM2_Init_Def.TIM_Prescaler = PWM_PRESCALER;
    TIM2_Init_Def.TIM_CounterMode = TIM_CounterMode_Up;
    TIM2_Init_Def.TIM_Period = PWM_PERIOD;
    TIM2_Init_Def.TIM_ClockDivision = PWM_CLK_DIV;
    TIM2_Init_Def.TIM_RepetitionCounter = 0;
    
    TIM_TimeBaseInit(PWM_TIMER, &TIM2_Init_Def);
    // Activate timer
    TIM_Cmd(PWM_TIMER, ENABLE); 

    // Configure timer output mode as PWM
    TIM_OCInitTypeDef TIM_OC_Init_Def;
    TIM_OC_Init_Def.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OC_Init_Def.TIM_Pulse = PWM_START_PULSE;
    TIM_OC_Init_Def.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC_Init_Def.TIM_OCPolarity = TIM_OCPolarity_High;

    // PWM for pin A1
    TIM_OC2Init(PWM_TIMER, &TIM_OC_Init_Def);
    //Activate pwm output for pin A1
    TIM_OC2PreloadConfig(PWM_TIMER, TIM_OCPreload_Enable);
    // PWM for pin A15
    TIM_OC1Init(PWM_TIMER, &TIM_OC_Init_Def);
    //Activate pwm output for pin A15
    TIM_OC1PreloadConfig(PWM_TIMER, TIM_OCPreload_Enable);
    // Configure GPIO pins A1 (right motor) and A15 (left motor)
    GPIO_InitTypeDef PWM_Init_Def;
    PWM_Init_Def.GPIO_Pin = PWM_RIGHT | PWM_LEFT;
    PWM_Init_Def.GPIO_Mode = GPIO_Mode_AF;
    PWM_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
    PWM_Init_Def.GPIO_OType = GPIO_OType_PP;
    PWM_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;

    GPIO_PinAFConfig(PWM_PORT, PWM_LEFT_SRC, PWM_PIN_AF);
    GPIO_PinAFConfig(PWM_PORT, PWM_RIGHT_SRC, PWM_PIN_AF);

    GPIO_Init(PWM_PORT, &PWM_Init_Def);
}


void pwm_stop(void){
    TIM_Cmd(PWM_TIMER, DISABLE);
}

void pwm_start(void){
    TIM_Cmd(PWM_TIMER, ENABLE);
}

static uint32_t duty_cycle_to_pulse_witdh(uint16_t duty_cycle){
    uint32_t t_pulse;
    if(duty_cycle == 0){
        return 0;
    }
    if(duty_cycle == 100){
        return PWM_PERIOD;
    }
    t_pulse = ((PWM_PERIOD + 1) * duty_cycle)/(100) - 1; 
    return t_pulse;
}

static void (*get_set_CCR_function(uint16_t pwm_channel))(TIM_TypeDef*, uint32_t){
    switch(pwm_channel){
        case PWM_RIGHT: return TIM_SetCompare2;
        case PWM_LEFT: return TIM_SetCompare1;
        default: return 0;
    }
}

void pwm_set_duty_cycle(uint16_t pwm_channel, uint16_t duty_cycle){
    uint32_t t_pulse;
    void (*set_CCR_function)(TIM_TypeDef*, uint32_t);
    set_CCR_function = get_set_CCR_function(pwm_channel);
    if(set_CCR_function == 0){
        return;
    }
    t_pulse = duty_cycle_to_pulse_witdh(duty_cycle);
    set_CCR_function(PWM_TIMER, t_pulse);
}

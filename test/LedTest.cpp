#include "TestHarness.h"
#include "MockSupport.h"

extern "C"{
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "stm32f4xx_gpio.h"
    #include "bsp.h"
    #include "led_driver.h"
}

TEST_GROUP(LedDriverTestGroup){

    GPIO_InitTypeDef LED_Init_Def;

    void setup(void){
        LED_Init_Def.GPIO_Pin = LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE;
        LED_Init_Def.GPIO_Mode = GPIO_Mode_OUT;
        LED_Init_Def.GPIO_OType = GPIO_OType_PP;
        LED_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
        LED_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
    }

    void teardown(void){
        mock().clear();
    }
};

TEST(LedDriverTestGroup, LedsInitProperly){
    mock().expectOneCall("RCC_AHB1PeriphClockCmd").onObject(this).withParameter("HW_Clock", RCC_AHB1Periph_GPIOD)
                                                                 .withParameter("MODE", ENABLE);
    mock().expectOneCall("GPIO_Init").onObject(this).withParameter("Port", LED_PORT)
                                                    .withParameter("Pin Init Struct", &LED_Init_Def);
    led_init();
    mock().checkExpectations();
}

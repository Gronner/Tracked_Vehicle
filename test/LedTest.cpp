#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include <stdio.h>
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "stm32f4xx_gpio.h"
    #include "bsp.h"
    #include "led_driver.h"
}

GPIO_InitTypeDef LED_Init_Def;

TEST_GROUP(LedDriverTestGroup){

    void setup(void){
        // Turn off memory leak detection:
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();

        LED_Init_Def.GPIO_Pin = LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE;
        LED_Init_Def.GPIO_Mode = GPIO_Mode_OUT;
        LED_Init_Def.GPIO_OType = GPIO_OType_PP;
        LED_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
        LED_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
    }

    void teardown(void){
        mock().clear();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

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


void RCC_AHB1PeriphClockCmd(uint32_t HW_Clock, FunctionalState State){
    mock().actualCall("RCC_AHB1PeriphClockCmd").withParameter("HW_Clock", HW_Clock)
                                                              .withParameter("STATE", State);
}

void GPIO_Init(GPIO_TypeDef* Port, GPIO_InitTypeDef* Pin_Init_Struct){
    mock().actualCall("GPIO_Init").withParameter("Port", LED_PORT)
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

TEST(LedDriverTestGroup, LedsInitProperly){
    mock_c()->installComparator("GPIO_InitType", gpio_is_equal, gpio_to_string);
    mock().expectOneCall("RCC_AHB1PeriphClockCmd").withParameter("HW_Clock", RCC_AHB1Periph_GPIOD)
                                                  .withParameter("STATE", ENABLE);
    mock().expectOneCall("GPIO_Init").withParameter("Port", LED_PORT)
                                     .withParameterOfType("GPIO_InitType", "Pin_Init_Struct", &LED_Init_Def);

    uint16_t all_leds = LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE;
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", all_leds);
    uint8_t led_status;
    led_status = led_init();
    mock().checkExpectations();
    CHECK_EQUAL(0, led_status);
}

TEST(LedDriverTestGroup, LedTurnOnOneLed){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    uint8_t led_status;
    led_status = led_turn_on(LED_GREEN);
    mock().checkExpectations();
    CHECK(0b00000001 & led_status);
}

TEST(LedDriverTestGroup, LedTurnOnTwoLeds){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_BLUE);
    uint8_t led_status;
    led_status = led_turn_on(LED_GREEN);
    CHECK(0b00000001 & led_status);
    led_status = led_turn_on(LED_BLUE);
    mock().checkExpectations();
    CHECK(0b00001001 & led_status);
}

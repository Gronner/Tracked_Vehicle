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

#include "test_gpio_mock.h"
#include "test_rcc_mock.h"

GPIO_InitTypeDef LED_Init_Def;
uint16_t all_leds = LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE;

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

TEST(LedDriverTestGroup, LedsInitProperly){
    mock_c()->installComparator("GPIO_InitType", gpio_is_equal, gpio_to_string);
    mock().expectOneCall("RCC_AHB1PeriphClockCmd").withParameter("HW_Clock", RCC_AHB1Periph_GPIOD)
                                                  .withParameter("STATE", ENABLE);
    mock().expectOneCall("GPIO_Init").withParameter("Port", LED_PORT)
                                     .withParameterOfType("GPIO_InitType", "Pin_Init_Struct", &LED_Init_Def);

    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", all_leds);
    uint8_t led_state;
    led_state = led_init();
    mock().checkExpectations();
    CHECK_EQUAL(0, led_state);
}

TEST(LedDriverTestGroup, LedTurnOnOneLed){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    uint8_t led_state;
    led_state = led_turn_on(LED_GREEN);
    mock().checkExpectations();
    CHECK(0b00000001 & led_state);
}

TEST(LedDriverTestGroup, LedTurnOnTwoLeds){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_BLUE);
    uint8_t led_state;
    led_state = led_turn_on(LED_GREEN);
    CHECK(0b00000001 & led_state);
    led_state = led_turn_on(LED_BLUE);
    mock().checkExpectations();
    CHECK(0b00001001 & led_state);
}

TEST(LedDriverTestGroup, LedTurnOnTwice){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    uint8_t led_state;
    led_state = led_turn_on(LED_GREEN);
    CHECK(0b00000001 & led_state);
    led_state = led_turn_on(LED_GREEN);
    CHECK(0b00000001 & led_state);
    mock().checkExpectations();
}

TEST(LedDriverTestGroup, LedTurnOffOneLed){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    uint8_t led_state;
    led_state = led_turn_on(LED_GREEN);
    CHECK(0b00000001 & led_state);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    led_state = led_turn_off(LED_GREEN);
    CHECK((0b00000001 & led_state) == 0);
    mock().checkExpectations();
}

TEST(LedDriverTestGroup, LedTurnOffTwoLeds){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_BLUE);
    uint8_t led_state;
    led_state = led_turn_on(LED_GREEN);
    CHECK(0b00000001 & led_state);
    led_state = led_turn_on(LED_BLUE);
    CHECK(0b00001001 & led_state);

    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_BLUE);
    led_state = led_turn_off(LED_BLUE);
    CHECK((0b00001000 & led_state) == 0);
    CHECK(0b00000001 & led_state);         //Check if LED_GREEN still on
    led_state = led_turn_off(LED_GREEN);
    CHECK((0b00000001 & led_state) == 0);

    mock().checkExpectations();
}

TEST(LedDriverTestGroup, LedTurnOffTwice){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    uint8_t led_state;
    led_state = led_turn_on(LED_GREEN);
    CHECK(0b00000001 & led_state);

    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    led_state = led_turn_off(LED_GREEN);
    CHECK((0b00000001 & led_state) == 0);
    led_state = led_turn_off(LED_GREEN);
    CHECK((0b00000001 & led_state) == 0);
    mock().checkExpectations();
}

TEST(LedDriverTestGroup, LedToggleOneLedOn){
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    uint8_t led_state;
    led_state = led_turn_off(LED_GREEN);
    CHECK((0b00000001 & led_state) == 0);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    led_state = led_toggle(LED_GREEN);
    CHECK(0b00000001 & led_state);
    mock().checkExpectations();
} 

TEST(LedDriverTestGroup, LedToggleOneLedTwice){
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    uint8_t led_state;
    led_state = led_turn_off(LED_GREEN);
    CHECK((0b00000001 & led_state) == 0);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    led_state = led_toggle(LED_GREEN);
    CHECK(0b00000001 & led_state);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    led_state = led_toggle(LED_GREEN);
    printf("%X", led_state);
    CHECK((0b00000001 & led_state) == 0);
    mock().checkExpectations();
}

TEST(LedDriverTestGroup, LedToogleTwoLeds){
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_BLUE);
    uint8_t led_state;
    led_state = led_turn_off(LED_GREEN);
    CHECK((0b00000001 & led_state) == 0);
    led_state = led_turn_off(LED_BLUE);
    CHECK((0b00001000 & led_state) == 0);

    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_BLUE);
    led_state = led_toggle(LED_GREEN);
    CHECK(0b00000001 & led_state);
    led_state = led_toggle(LED_BLUE);
    CHECK(0b00001000 & led_state);
    mock().checkExpectations();
}


TEST(LedDriverTestGroup, LedToogleTwoLedsTwice){
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_BLUE);
    uint8_t led_state;
    led_state = led_turn_off(LED_GREEN);
    CHECK((0b00000001 & led_state) == 0);
    led_state = led_turn_off(LED_BLUE);
    CHECK((0b00001000 & led_state) == 0);

    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_BLUE);
    led_state = led_toggle(LED_GREEN);
    CHECK(0b00000001 & led_state);
    led_state = led_toggle(LED_BLUE);
    CHECK(0b00001000 & led_state);

    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", LED_BLUE);
    led_state = led_toggle(LED_GREEN);
    CHECK((0b00000001 & led_state) == 0);
    led_state = led_toggle(LED_BLUE);
    CHECK((0b00001000 & led_state) == 0);
    mock().checkExpectations();
}

TEST(LedDriverTestGroup, LedTurnOnAll){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", all_leds);
    uint8_t led_state;
    led_state = led_turn_on_all();
    CHECK(0b00001111 & led_state);
    mock().checkExpectations();
}

TEST(LedDriverTestGroup, LedTurnOffAll){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", all_leds);
    uint8_t led_state;
    led_state = led_turn_on_all();
    CHECK(0b00001111 & led_state);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                          .withParameter("GPIO_Pin", all_leds);
    led_state = led_turn_off_all();
    CHECK((0b00001111 & led_state) == 0);
    mock().checkExpectations();
}

TEST(LedDriverTestGroup, GetAllOffState){
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", all_leds);
    uint8_t led_state;
    led_state = led_turn_off_all();
    CHECK((0 & led_state) == 0);
    led_state = led_get_state();
    CHECK((0 & led_state) == 0);
    mock().checkExpectations();
}

TEST(LedDriverTestGroup, GetAllOnState){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", all_leds);
    uint8_t led_state;
    led_state = led_turn_on_all();
    CHECK(0b00001111 & led_state);
    led_state = led_get_state();
    CHECK(0b00001111 & led_state);
    mock().checkExpectations();
}

TEST(LedDriverTestGroup, GetAlternatingOnOff){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_GREEN);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_RED);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_ORANGE);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LED_PORT)
                                        .withParameter("GPIO_Pin", LED_BLUE);
    led_turn_on(LED_GREEN);
    led_turn_on(LED_RED);
    led_turn_off(LED_ORANGE);
    led_turn_off(LED_BLUE);
    uint8_t led_state;
    led_state = led_get_state();
    CHECK(0b00000101 & led_state);
    mock().checkExpectations();
}

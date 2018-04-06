#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "stm32f4xx_gpio.h"
    #include "bsp.h"
    #include "motor_driver.h"
}

#include "test_rcc_mock.h"
#include "test_gpio_mock.h"

GPIO_InitTypeDef Ctrl_Init_Def;
uint16_t all_ctrl_pins = DC_CTRL_PIN_LF | DC_CTRL_PIN_LR | DC_CTRL_PIN_RF | DC_CTRL_PIN_RR;
TEST_GROUP(DCMotDriverTestGroup){
    void setup(void){
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
        Ctrl_Init_Def.GPIO_Pin = all_ctrl_pins;
        Ctrl_Init_Def.GPIO_Mode = GPIO_Mode_OUT;
        Ctrl_Init_Def.GPIO_OType = GPIO_OType_PP;
        Ctrl_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
        Ctrl_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
    }

    void teardown(void){
        mock().clear();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

/* For a passing test the folloing has to happen:
 * Enable peripheral clock to GPIOs
 * Call GPIO Init with proper settings
 */
TEST(DCMotDriverTestGroup, DCMotInitProperly){
    mock_c()->installComparator("GPIO_InitType", gpio_is_equal, gpio_to_string);

    mock().expectOneCall("RCC_AHB1PeriphClockCmd").withParameter("HW_Clock", RCC_AHB1Periph_GPIOD)
                                                  .withParameter("STATE", ENABLE);
    mock().expectOneCall("GPIO_Init").withParameter("Port", DC_CTRL_PORT)
                                     .withParameterOfType("GPIO_InitType",
                                                         "Pin_Init_Struct",
                                                         &Ctrl_Init_Def);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", DC_CTRL_PORT)
                                          .withParameter("GPIO_Pin", all_ctrl_pins);
    motor_init();
    mock().checkExpectations();
}

// Vehicle is already moving forward 
TEST(DCMotDriverTestGroup, DCMotChangeDirReverseLeft){
    mock().expectOneCall("GPIO_ReadOutputData").withParameter("Port", DC_CTRL_PORT);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", DC_CTRL_PORT)
                                          .withParameter("GPIO_Pin", DC_CTRL_PIN_LF);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", DC_CTRL_PORT)
                                        .withParameter("GPIO_Pin", DC_CTRL_PIN_LR);

    motor_change_direction(DC_LEFT, DC_DIR_REVERSE);
    mock().checkExpectations();
}

// Vehicle is already moving forward 
TEST(DCMotDriverTestGroup, DCMotChangeDirForwardLeft){
    mock().expectOneCall("GPIO_ReadOutputData").withParameter("Port", DC_CTRL_PORT);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", DC_CTRL_PORT)
                                        .withParameter("GPIO_Pin", DC_CTRL_PIN_LF);
    motor_change_direction(DC_LEFT, DC_DIR_FORWARD);
    mock().checkExpectations();
}

// Vehicle is already moving forward 
TEST(DCMotDriverTestGroup, DCMotChangeDirReverseRight){
    mock().expectOneCall("GPIO_ReadOutputData").withParameter("Port", DC_CTRL_PORT);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", DC_CTRL_PORT)
                                          .withParameter("GPIO_Pin", DC_CTRL_PIN_RF);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", DC_CTRL_PORT)
                                        .withParameter("GPIO_Pin", DC_CTRL_PIN_RR);

    motor_change_direction(DC_RIGHT, DC_DIR_REVERSE);
    mock().checkExpectations();
}

// Vehicle is already moving forward 
TEST(DCMotDriverTestGroup, DCMotChangeDirForwardRight){
    mock().expectOneCall("GPIO_ReadOutputData").withParameter("Port", DC_CTRL_PORT);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", DC_CTRL_PORT)
                                        .withParameter("GPIO_Pin", DC_CTRL_PIN_RF);
    motor_change_direction(DC_RIGHT, DC_DIR_FORWARD);
    mock().checkExpectations();
}

TEST(DCMotDriverTestGroup, DCMotChangeDirReverseBoth){
    mock().expectOneCall("GPIO_ReadOutputData").withParameter("Port", DC_CTRL_PORT);
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", DC_CTRL_PORT)
                                          .withParameter("GPIO_Pin",
                                                         DC_CTRL_PIN_LF | DC_CTRL_PIN_RF);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", DC_CTRL_PORT)
                                        .withParameter("GPIO_Pin",
                                                       DC_CTRL_PIN_LR | DC_CTRL_PIN_RR);
    motor_change_direction(DC_BOTH, DC_DIR_REVERSE);
    mock().checkExpectations();
}

TEST(DCMotDriverTestGroup, DCMotChangeDirForwardBoth){
    mock().expectOneCall("GPIO_ReadOutputData").withParameter("Port", DC_CTRL_PORT);
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", DC_CTRL_PORT)
                                        .withParameter("GPIO_Pin",
                                                       DC_CTRL_PIN_LF | DC_CTRL_PIN_RF);
    motor_change_direction(DC_BOTH, DC_DIR_FORWARD);
    mock().checkExpectations();
}

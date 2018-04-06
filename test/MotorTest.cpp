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
 * Enable peripheral clock to timer and GPIOs
 * Call Timer Init with proper settings
 * Call PWM Init twice with proper settings
 * Call GPIO Init with proper settings
 */
TEST(DCMotDriverTestGroup, DCMotInitProperly){
    FAIL("This is intended to fail");
}

#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include <stdio.h>
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "stm32f4xx_gpio.h"
    #include "bsp.h"
    #include "lcd_driver.h"
}

#include "test_gpio_mock.h"
#include "test_rcc_mock.h"

GPIO_InitTypeDef LCD_Init_Def;

TEST_GROUP(LcdDriverTestGroup){

    void setup(void){
    }

    void teardown(void){
    }
};

TEST(LcdDriverTestGroup, InitLcdProperly){
    FAIL("This is supposed to fail");
}

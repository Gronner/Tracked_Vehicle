#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include <stdio.h>
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "stm32f4xx_gpio.h"
    #include "stm32f4xx_i2c.h"
    #include "bsp.h"
    #include "i2c_driver.h"
}

#include "test_rcc_mock.h"
#include "test_gpio_mock.h"
#include "test_i2c_mock.h"

GPIO_InitTypeDef I2C_Pin_Init_Def;
I2C_InitTypeDef I2C_Init_Def;

TEST_GROUP(I2CDriverTestGroup){

    void setup(void){
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
        // Pin setup
        I2C_Pin_Init_Def.GPIO_Pin = I2C_SCL | I2C_SDA;
        I2C_Pin_Init_Def.GPIO_Mode = GPIO_Mode_AF;
        I2C_Pin_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
        I2C_Pin_Init_Def.GPIO_OType = GPIO_OType_OD;
        I2C_Pin_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;

        // I2C setup
        I2C_Init_Def.I2C_ClockSpeed = I2C_CLOCK_SPEED;
        I2C_Init_Def.I2C_Mode = I2C_Mode_I2C;
        I2C_Init_Def.I2C_DutyCycle = I2C_DutyCycle_16_9;
        I2C_Init_Def.I2C_OwnAddress1 = I2C_OWN_ADRESS;
        I2C_Init_Def.I2C_Ack = I2C_Ack_Enable;
        I2C_Init_Def.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    }

    void teardown(void){
        mock().clear();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

/* For a passing test the following has to happen:
 * Enable peripheral clock to I2C1 and GPIOs
 * Call I2C Init with proper settings
 * Call GPIO Init with proper settings
 */
TEST(I2CDriverTestGroup, I2CInitProperly){
    mock_c()->installComparator("GPIO_InitType", gpio_is_equal, gpio_to_string);
    mock_c()->installComparator("I2C_InitType", i2c_is_equal, i2c_to_string);
    // Check for clocks
    mock().expectOneCall("RCC_APB1PeriphClockCmd").withParameter("HW_Clock", RCC_APB1Periph_I2C1)
                                                  .withParameter("STATE", ENABLE);
    mock().expectOneCall("RCC_AHB1PeriphClockCmd").withParameter("HW_Clock", RCC_AHB1Periph_GPIOB)
                                                  .withParameter("STATE", ENABLE);
    // Check for GPIO
    mock().expectOneCall("GPIO_PinAFConfig").withParameter("Port", I2C_PORT)
                                            .withParameter("Pins", I2C_SCL_SRC)
                                            .withParameter("AF_Sel", GPIO_AF_I2C1);
    mock().expectOneCall("GPIO_PinAFConfig").withParameter("Port", I2C_PORT)
                                            .withParameter("Pins", I2C_SDA_SRC)
                                            .withParameter("AF_Sel", GPIO_AF_I2C1);
    mock().expectOneCall("GPIO_Init").withParameter("Port", I2C_PORT)
                                     .withParameterOfType("GPIO_InitType",
                                                          "Pin_Init_Struct", 
                                                          &I2C_Pin_Init_Def);

    // Check for I2C
    mock().expectOneCall("I2C_Init").withParameter("I2C", I2C_BUS)
                                    .withParameterOfType("I2C_InitType",
                                                         "I2C_Init_Struct",
                                                         &I2C_Init_Def);
    mock().expectOneCall("I2C_Cmd").withParameter("I2C", I2C_BUS)
                                   .withParameter("STATE", ENABLE);

    i2c_init();
    mock().checkExpectations();
}

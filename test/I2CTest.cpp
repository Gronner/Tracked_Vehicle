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

// Test definitions
// LSM303DLHC
#define LSM_ACC_ADR 0x32
#define LSM_MAG_ADR 0x3C
#define LSM_CTR_ODR_50HZ 0x40
#define LSM_CTR_AXES_ENABLE 0x07
// Register Map
#define LSM_CTR_SADR 0x20
#define LSM_ACC_OUT_X_L 0x28

GPIO_InitTypeDef I2C_Pin_Init_Def;
I2C_InitTypeDef I2C_Init_Def;

TEST_GROUP(I2CDriverTestGroup){

    void setup(void){
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
        // Pin setup
        I2C_Pin_Init_Def.GPIO_Pin = I2C_SCL | I2C_SDA;
        I2C_Pin_Init_Def.GPIO_Mode = GPIO_Mode_AF;
        I2C_Pin_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
        I2C_Pin_Init_Def.GPIO_OType = GPIO_OType_PP;
        I2C_Pin_Init_Def.GPIO_PuPd = GPIO_PuPd_DOWN;

        // I2C setup
        I2C_Init_Def.I2C_ClockSpeed = I2C_CLOCK_SPEED;
        I2C_Init_Def.I2C_Mode = I2C_Mode_I2C;
        I2C_Init_Def.I2C_DutyCycle = I2C_DutyCycle_2;
        I2C_Init_Def.I2C_OwnAddress1 = I2C_OWN_ADDRESS;
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

TEST(I2CDriverTestGroup, I2CSendData){
    mock().expectOneCall("I2C_GenerateSTART").withParameter("I2C", I2C_BUS)
                                             .withParameter("STATE", ENABLE);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_MODE_SELECT);
    mock().expectOneCall("I2C_Send7bitAddress").withParameter("I2C", I2C_BUS)
                                               .withParameter("Address", LSM_ACC_ADR)
                                               .withParameter("Direction", I2C_Direction_Transmitter);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    mock().expectOneCall("I2C_SendData").withParameter("I2C", I2C_BUS)
                                        .withParameter("Data", LSM_CTR_SADR);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    mock().expectOneCall("I2C_SendData").withParameter("I2C", I2C_BUS)
                                        .withParameter("Data", LSM_CTR_ODR_50HZ | LSM_CTR_AXES_ENABLE);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    mock().expectOneCall("I2C_GenerateSTOP").withParameter("I2C", I2C_BUS)
                                             .withParameter("STATE", ENABLE);

    uint8_t data_buffer[1] = {LSM_CTR_ODR_50HZ | LSM_CTR_AXES_ENABLE};
    i2c_write(LSM_ACC_ADR, LSM_CTR_SADR, data_buffer, 1);
    mock().checkExpectations();
}

TEST(I2CDriverTestGroup, I2CReadData){
    mock().expectOneCall("I2C_GenerateSTART").withParameter("I2C", I2C_BUS)
                                             .withParameter("STATE", ENABLE);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_MODE_SELECT);
    mock().expectOneCall("I2C_Send7bitAddress").withParameter("I2C", I2C_BUS)
                                               .withParameter("Address", LSM_ACC_ADR)
                                               .withParameter("Direction", I2C_Direction_Transmitter);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    mock().expectOneCall("I2C_SendData").withParameter("I2C", I2C_BUS)
                                        .withParameter("Data", LSM_ACC_OUT_X_L);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_BYTE_TRANSMITTED);

    mock().expectOneCall("I2C_GenerateSTART").withParameter("I2C", I2C_BUS)
                                             .withParameter("STATE", ENABLE);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_MODE_SELECT);
    mock().expectOneCall("I2C_Send7bitAddress").withParameter("I2C", I2C_BUS)
                                               .withParameter("Address", LSM_ACC_ADR)
                                               .withParameter("Direction", I2C_Direction_Receiver);
    mock().expectOneCall("I2C_GetFlagStatus").withParameter("I2C", I2C_BUS)
                                             .withParameter("Flag", I2C_FLAG_ADDR);
    mock().expectOneCall("I2C_AcknowledgeConfig").withParameter("I2C", I2C_BUS)
                                                 .withParameter("STATE", DISABLE);
    mock().expectOneCall("I2C_GenerateSTOP").withParameter("I2C", I2C_BUS)
                                            .withParameter("STATE", ENABLE);
    mock().expectOneCall("I2C_ReceiveData").withParameter("I2C", I2C_BUS);
    mock().expectOneCall("I2C_AcknowledgeConfig").withParameter("I2C", I2C_BUS)
                                                 .withParameter("STATE", ENABLE);
    uint8_t data_buffer[1] = {0};
    i2c_read(LSM_ACC_ADR, LSM_ACC_OUT_X_L, data_buffer, 1);
    CHECK(data_buffer[0] == 0xAA);
    mock().checkExpectations(); 
}

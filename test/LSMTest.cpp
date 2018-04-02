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
    #include "lsm_driver.h"
}

#include "test_rcc_mock.h"
#include "test_gpio_mock.h"
#include "test_i2c_mock.h"

TEST_GROUP(LSMDriverTestGroup){

    void setup(void){
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
    }

    void teardown(void){
        mock().clear();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

void mock_i2c_write(uint8_t adr, uint8_t subadr, uint8_t* data){
    mock().expectOneCall("I2C_GenerateSTART").withParameter("I2C", I2C_BUS)
                                             .withParameter("STATE", ENABLE);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_MODE_SELECT);
    mock().expectOneCall("I2C_Send7bitAddress").withParameter("I2C", I2C_BUS)
                                               .withParameter("Address", adr)
                                               .withParameter("Direction", I2C_Direction_Transmitter);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
    mock().expectOneCall("I2C_SendData").withParameter("I2C", I2C_BUS)
                                        .withParameter("Data", subadr);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    mock().expectOneCall("I2C_SendData").withParameter("I2C", I2C_BUS)
                                        .withParameter("Data", data[0]);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_BYTE_TRANSMITTED);
    mock().expectOneCall("I2C_GenerateSTOP").withParameter("I2C", I2C_BUS)
                                             .withParameter("STATE", ENABLE);
}

/* For a passing test the following has to happen:
 * Set data rate, enable X and y axes in control register 1
 * Set Full Scale selection to 4g in control register 4
 */
TEST(LSMDriverTestGroup, LSMInitProperly){
    uint8_t data_buffer[1] = {LSM_ACC_50HZ | LSM_ACC_X_EN | LSM_ACC_Y_EN};
    mock_i2c_write(LSM_ACC_ADR, LSM_ACC_CTR1_SADR, data_buffer);
    data_buffer[0] = (LSM_ACC_4G);
    mock_i2c_write(LSM_ACC_ADR, LSM_ACC_CTR4_SADR, data_buffer);
    lsm_init();
    mock().checkExpectations();
}

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

void mock_i2c_read(uint8_t adr, uint8_t subadr){
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

    mock().expectOneCall("I2C_GenerateSTART").withParameter("I2C", I2C_BUS)
                                             .withParameter("STATE", ENABLE);
    mock().expectOneCall("I2C_CheckEvent").withParameter("I2C", I2C_BUS)
                                          .withParameter("Event", I2C_EVENT_MASTER_MODE_SELECT);
    mock().expectOneCall("I2C_Send7bitAddress").withParameter("I2C", I2C_BUS)
                                               .withParameter("Address", adr)
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
}

/* For a passing test the following has to happen:
 * Set data rate, enable X and y axes in control register 1
 * Set Full Scale selection to 4g in control register 4
 */
TEST(LSMDriverTestGroup, LSMInitProperly){
    uint8_t data_buffer[1] = {LSM_ACC_50HZ | LSM_ACC_ALL_AX_EN};
    mock_i2c_write(LSM_ACC_ADR, LSM_ACC_CTR1_SADR, data_buffer);
    data_buffer[0] = (LSM_ACC_4G);
    mock_i2c_write(LSM_ACC_ADR, LSM_ACC_CTR4_SADR, data_buffer);
    lsm_init();
    mock().checkExpectations();
}

TEST(LSMDriverTestGroup, LSMReadXAxis){
    mock_i2c_read(LSM_ACC_ADR, LSM_ACC_X_L_SADR); // Expect read lower byte
    mock_i2c_read(LSM_ACC_ADR, LSM_ACC_X_H_SADR); // Expect read upper byte
    uint16_t axis_data;
    axis_data = lsm_read_axis('x');
    mock().checkExpectations();
    CHECK(axis_data == 0xAAAA);
}
    
TEST(LSMDriverTestGroup, LSMReadYAxis){
    mock_i2c_read(LSM_ACC_ADR, LSM_ACC_Y_L_SADR); // Expect read lower byte
    mock_i2c_read(LSM_ACC_ADR, LSM_ACC_Y_H_SADR); // Expect read upper byte
    uint16_t axis_data;
    axis_data = lsm_read_axis('y');
    mock().checkExpectations();
    CHECK(axis_data == 0xAAAA);
}

TEST(LSMDriverTestGroup, LSMReadZAxis){
    mock_i2c_read(LSM_ACC_ADR, LSM_ACC_Z_L_SADR); // Expect read lower byte
    mock_i2c_read(LSM_ACC_ADR, LSM_ACC_Z_H_SADR); // Expect read upper byte
    uint16_t axis_data;
    axis_data = lsm_read_axis('z');
    mock().checkExpectations();
    CHECK(axis_data == 0xAAAA);
}

TEST(LSMDriverTestGroup, LSMReadWrongAxis){
    uint16_t axis_data;
    axis_data = lsm_read_axis('w');
    CHECK(axis_data = 0xFFFF);
}

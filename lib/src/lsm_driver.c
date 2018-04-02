#include "i2c_driver.h"
#include "lsm_driver.h"

/* This functions assumes the I2C bus at PB6/PB9 is already initialized
 */
void lsm_init(void){
    uint8_t data_buffer[1] = {0x00};
    data_buffer[0] = (LSM_ACC_50HZ | LSM_ACC_X_EN | LSM_ACC_Y_EN);
    i2c_write(LSM_ACC_ADR, LSM_ACC_CTR1_SADR, data_buffer, 1);
    data_buffer[0] = LSM_ACC_4G;
    i2c_write(LSM_ACC_ADR, LSM_ACC_CTR4_SADR, data_buffer, 1);
}

#include "lsm_driver.h"
#include "i2c_driver.h"

/* This functions assumes the I2C bus at PB6/PB9 is already initialized
 */
void lsm_init(void) {
  uint8_t data_buffer[1] = {0x00};
  data_buffer[0] = (LSM_ACC_50HZ | LSM_ACC_ALL_AX_EN);
  i2c_write(LSM_ACC_ADR, LSM_ACC_CTR1_SADR, data_buffer, 1);
  data_buffer[0] = LSM_ACC_4G;
  i2c_write(LSM_ACC_ADR, LSM_ACC_CTR4_SADR, data_buffer, 1);
}

uint16_t lsm_read_axis(char axis) {
  uint16_t axis_data = 0;
  uint8_t data_buffer[1] = {0x00};
  uint8_t register_lower_address;
  uint8_t register_upper_address;
  switch (axis) {
    case 'x':
      register_lower_address = LSM_ACC_X_L_SADR;
      register_upper_address = LSM_ACC_X_H_SADR;
      break;
    case 'y':
      register_lower_address = LSM_ACC_Y_L_SADR;
      register_upper_address = LSM_ACC_Y_H_SADR;
      break;
    case 'z':
      register_lower_address = LSM_ACC_Z_L_SADR;
      register_upper_address = LSM_ACC_Z_H_SADR;
      break;
    default:
      return 0xFFF;
  }
  // Read lower byte
  i2c_read(LSM_ACC_ADR, register_lower_address, data_buffer, 1);
  axis_data = data_buffer[0];
  // Read upper byte
  i2c_read(LSM_ACC_ADR, register_upper_address, data_buffer, 1);
  axis_data |= data_buffer[0] << 8;
  return axis_data;
}

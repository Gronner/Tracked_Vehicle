#ifndef __I2C_DRIVER_H_
#define __I2C_DRIVER_H_
#include <stdint.h>

void i2c_init(void);
void i2c_write(uint8_t address, uint8_t subaddress, uint8_t* data_buffer, uint8_t buffer_len);

#endif // __I2C_DRIVER_H_

#ifndef __I2C_DRIVER_H_
#define __I2C_DRIVER_H_
#include <stdint.h>

/* @brief Initializes the I2C1 peripheral in a configuration, so it can 
 * communicate with the LSM303DLHC device.
 */
void i2c_init(void);

/* @brief Writes one byte of data to a register of a device on the I2C1 bus.
 * The MCU acts in master mode.
 * @param address Address of the slave device.
 * @param subaddress Register address of the register the write access is
 * happening.
 * @param data_buffer Array of the data to be written. The function only
 * supports a single byte of data.
 * @param buffer_len Length of the data array.
 */
void i2c_write(uint8_t address, uint8_t subaddress, uint8_t* data_buffer, uint8_t buffer_len);

/* @brief Reads one byte of data from a device on the I2C1 bus.
 * @param address Address of the slave device.
 * @param subaddress Register address of the register the read access is
 * happening.
 * @param data_buffer Array where the read data is stored. The functin only
 * supports a single byte of data.
 * @param buffer_len Length of the data array.
 */
void i2c_read(uint8_t address, uint8_t subaddress, uint8_t* data_buffer, uint8_t buffer_len);

#endif // __I2C_DRIVER_H_

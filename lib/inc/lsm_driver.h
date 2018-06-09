#ifndef __LSM_DRIVER_H_
#define __LSM_DRIVER_H_
#include <stdint.h>

/* ---- Address definitons ---- */
#define LSM_ACC_ADR 0x32  // Accelerometer slave address
#define LSM_MAG_ADR 0x3C  // 3D compass slave address
// ----  Register map ---- //
// Conrol register
#define LSM_ACC_CTR1_SADR 0x20
#define LSM_ACC_CTR2_SADR 0x21
#define LSM_ACC_CTR3_SADR 0x22
#define LSM_ACC_CTR4_SADR 0x23
// Accelerometer axis address
#define LSM_ACC_X_L_SADR 0x28  // X-axis lower
#define LSM_ACC_X_H_SADR 0x29  // X-axis high
#define LSM_ACC_Y_L_SADR 0x2A  // Y-axis lower
#define LSM_ACC_Y_H_SADR 0x2B  // Y-axis high
#define LSM_ACC_Z_L_SADR 0x2C  // Z-axis lower
#define LSM_ACC_Z_H_SADR 0x2D  // Z-axis high
// ---- Register value definitions ---- //
#define LSM_ACC_50HZ 0x04      // Set data rate to 50Hz
#define LSM_ACC_X_EN 0x20      // Enable X axis
#define LSM_ACC_Y_EN 0x40      // Enable Y axis
#define LSM_ACC_Z_EN 0x80      // Enable Y axis
#define LSM_ACC_ALL_AX_EN 0xE  // Enable all axes
#define LSM_ACC_4G 0x08        // Use 4g as full-scale

/* @brief Initializes the LSM303DLHC in 50Hz mode and enables all three
 * accelerometer axes.
 */
void lsm_init(void);
/* @brief Reads out upper and lower byte of the accelerometer's value in one
 * axis.
 * @param axis Char specifying the axis. Is either 'x', 'y' or 'z'.
 */
uint16_t lsm_read_axis(char axis);
#endif  // __LSM_DRIVER_H_

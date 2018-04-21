#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H
#include <stdint.h>

#define DC_SAFE_TIME 3 // Reverse direction delay in ms
#define DC_LEFT 1
#define DC_RIGHT 2
#define DC_BOTH 3
#define DC_DIR_FORWARD 1
#define DC_DIR_REVERSE 2

/* @brief Initializes the control pins of the motor driver. */
void motor_init(void);

/* @brief Handles reversing the direction of the motors. Can also be used to 
 * start driving in one direction.
 * @param side Integer value specifying the side to be reversed. Can either be
 * 1 - left, 2 - right, 3 - both.
 * @param direction Integer value specifying the new direction of the motor. Can
 * either be 1 - forward or 2 - reverse.
 */
void motor_change_direction(uint8_t side, uint8_t direction);

/* @brief Changes the voltage supplied to the motors. Controls the PWM module
 * @param side Integer value specifying the side that is controlled. Can either
 * be 1- left, 2 - right, 3 - both.
 * @param Voltage supplied to the motors. Values below 2V won't result in 
 * motor movement.
 */
void motor_set_drive_v(uint8_t side, float voltage);

#endif // __MOTOR_DRIVER_H

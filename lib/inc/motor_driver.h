#ifndef __MOTOR_DRIVER_H
#define __MOTOR_DRIVER_H
#include <stdint.h>

#define DC_SAFE_TIME 3 // Reverse direction delay in ms
#define DC_LEFT 1
#define DC_RIGHT 2
#define DC_BOTH 3
#define DC_DIR_FORWARD 1
#define DC_DIR_REVERSE 2

void motor_init(void);
void motor_change_direction(uint8_t side, uint8_t direction);

#endif // __MOTOR_DRIVER_H

#ifndef __PWD_DRIVER_H_
#define __PWD_DRIVER_H_
#include <stdint.h>

void pwm_init(void);
void pwm_stop(void);
void pwm_start(void);
void pwm_set_duty_cycle(uint16_t pwm_channel, uint16_t duty_cycle);
#endif // __PWD_DRIVER_H_

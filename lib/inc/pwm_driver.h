#ifndef __PWD_DRIVER_H_
#define __PWD_DRIVER_H_
#include <stdint.h>

/* @brief Initializes timer 2 and configures output channel 1 and 2 to PWM1 
 * mode. 
 */
void pwm_init(void);

/* @brief Stops the counter of the timer for both output channel. */
void pwm_stop(void);

/* @brief Starts the counter of the timer for both output channel. */
void pwm_start(void);

/* @brief Changes the duty cycle of one pwm channel
 * @param pwm_channel Channel which needs to be adjusted. Uses the pin bitmask
 * as selector.
 * @param duty_cycle New duty cycle in percentage. Possible values from 0% to 101%
 */
void pwm_set_duty_cycle(uint16_t pwm_channel, uint16_t duty_cycle);

/* @brief Get the duty cycle of one pwm channel.
 * @param pwm_channel Channel that should be queried. Uses the pin bitmask as 
 * selector.
 */
uint16_t pwm_get_duty_cycle(uint16_t pwm_channel);
#endif // __PWD_DRIVER_H_

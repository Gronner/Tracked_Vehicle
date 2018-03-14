#ifndef __LED_DRIVER_H_
#define __LED_DRIVER_H_

uint8_t led_init(void);
uint8_t led_turn_on(uint16_t led);
uint8_t led_turn_off(uint16_t led);
uint8_t led_toggle(uint16_t led);
uint8_t led_turn_on_all(void);
uint8_t led_turn_off_all(void);
uint8_t led_get_state(void);
#endif // __LED_DRIVER_H_

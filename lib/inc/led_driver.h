#ifndef __LED_DRIVER_H_
#define __LED_DRIVER_H_

/* @brief Initialize Port D, so the four on board LEDs can be interacted with.
 * Their initial state is off.
 */
uint8_t led_init(void);

/* @brief Turn on one or more of the on board LEDs.
 * @param led Takes the bitmask of the LED's pin. Multiple pins can be passed
 * using the or Operator |
 */
uint8_t led_turn_on(uint16_t led);

/* @brief Turn off one or more of the on board LEDs.
 * @param led Takes the bitmask of the LED's pin. Multiple pins can be passed
 * using the or Operator |
 */
uint8_t led_turn_off(uint16_t led);

/* @brief Toggles one or more of the on board LEDs. So turning on/off LEDs that
 * are off/on.
 * @param led Takes the bitmasl of the LED's pin. Multiple pins can be passed
 * using the or Operator.
 */
uint8_t led_toggle(uint16_t led);

/* @brief Turns on all four on board LED's. */
uint8_t led_turn_on_all(void);

/* @brief Turns off all four on board LED's */
uint8_t led_turn_off_all(void);

/* @brief Queries the intern state of the four on board LED's.
 * @returns LED's state as bitmask where the first bit refers to the LED on
 * Pin12 and so on.
 */
uint8_t led_get_state(void);
#endif  // __LED_DRIVER_H_

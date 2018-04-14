#ifndef __LCD_DRIVER_H_
#define __LCD_DRIVER_H_
#include <stdint.h>

/* @brief Initialize the pins 6 to 11 on port D to write to an Hitachy 
 * HD44780U LCD driver. The communication runs in 4-bit mode with a 2x16 dispaly.
 */
void lcd_init(void);

/* @brief Writes a command to the LCD driver.
 * @param cmd Byte representing a valid command for the HD44780U.
 */
void lcd_write_cmd(uint8_t cmd);

/* @brief Writes a character to the LCD driver.
 * @param c Byte character, that mostly follows the ASCII codes. Above 127 a
 * japanese character set is used.
 */
void lcd_write_char(char c);

/* @brief Places the cursor on the LCD. If the value is outside of the allowed 
 * range it gets placed at the beginning of the next row.
 * @param row Selects the row the cursor should be placed in. Is either 0 or 1.
 * @param column Selects the column the cursor should be placed inn. Is between
 * 0 ... 15.
 */
void lcd_cursor_put(uint8_t row, uint8_t column);

/* @brief Clears the LCD screen. */
void lcd_clear(void);

/* @brief Implements a put function for writing to the screen that handles
 * special characters.
 * @param c Characters to be printed.
 */
void lcd_put(char c);

/* @brief Prints a string to the LCD screen.
 * @param _string The string to be printed to the screen.
 * @param str_len Length of the string.
 */
void lcd_print_string(char* _string, uint8_t str_len);

/* @brief Prints a number to the screen using an itoa function.
 * @param number Number to be printed to the screen.
 */
void lcd_print_integer(int32_t number);

#endif // __LCD_DRIVER_H_

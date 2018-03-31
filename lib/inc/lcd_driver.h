#ifndef __LCD_DRIVER_H_
#define __LCD_DRIVER_H_
#include <stdint.h>

void lcd_init(void);
void lcd_write_cmd(uint8_t cmd);
void lcd_write_char(char c);
void lcd_cursor_put(uint8_t row, uint8_t column);
void lcd_clear(void);
void lcd_put(char c);
void lcd_print_string(char* _string, uint8_t str_len);
void lcd_print_integer(int32_t number);

#endif // __LCD_DRIVER_H_

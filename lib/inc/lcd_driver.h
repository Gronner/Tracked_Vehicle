#ifndef __LCD_DRIVER_H_
#define __LCD_DRIVER_H_
#include <stdint.h>

void lcd_init(void);
void lcd_write_cmd(uint8_t cmd);
void lcd_write_char(char c);

#endif // __LCD_DRIVER_H_

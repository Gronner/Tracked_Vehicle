#include "lcd_driver.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "bsp.h"

#define ALL_DATA_PINS ((uint16_t)(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7))
#define CHECK_BIT(byte, bit) ((byte) & (1 << (bit)))

/* Upper nibble = row
 * 0x0X -> 1st row
 * 0x4X -> 2nd row
 * Lower nibble = column
 * 0xX0 -> 1st row
 * 0xXF -> 16th row
 */
static uint8_t cursor_pos; 

static void write_nibble(uint8_t nibble);
static void write_byte(uint8_t byte);
static void update_cursor_pos(void);

void lcd_init(void){
    // Activate GPIO port D periperhal clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    // Configure Pins
    GPIO_InitTypeDef LCD_Init_Def;
    LCD_Init_Def.GPIO_Pin = (LCD_RS | LCD_EN | ALL_DATA_PINS);
    LCD_Init_Def.GPIO_Mode = GPIO_Mode_OUT;
    LCD_Init_Def.GPIO_OType = GPIO_OType_PP;
    LCD_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
    LCD_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(LCD_PORT, &LCD_Init_Def);

    uint16_t counter;
    for(counter = 0; counter < UINT16_MAX/2; counter++); // Wait so lcd driver can stabalize

    // Set all data pins to low
    GPIO_ResetBits(LCD_PORT, ALL_DATA_PINS);
    // Set RS to command
    GPIO_ResetBits(LCD_PORT, LCD_RS);
    // Set EN to 1
    GPIO_ResetBits(LCD_PORT, LCD_EN);
    // Write 8-bit command 3 times
    uint8_t i;
    for(i = 0; i < 3; i++){
        write_nibble(LCD_8b);
    }
    // Write 4-bit command
    write_nibble(LCD_4b);
    // Write display mode command TODO: Replace later on with write command
    lcd_write_cmd(LCD_DM);
    // Send active display command
    lcd_write_cmd(LCD_ACT);
    // Clear LCD
    lcd_write_cmd(LCD_CLR);

    cursor_pos = 0x00; // Cursor in first row, first column
}

static void write_nibble(uint8_t nibble){
    uint8_t i;
    GPIO_SetBits(LCD_PORT, LCD_EN);
    for(i = 0;i < 4; i++){
        uint16_t pin = 1 << (LCD_SHIFT + i);
        if(CHECK_BIT(nibble, i)){
            GPIO_SetBits(LCD_PORT, pin);
        }
        else{
            GPIO_ResetBits(LCD_PORT, pin);
        }
    }
    uint16_t counter;
    for(counter = 0; counter < UINT16_MAX/2; counter++); // Wait so lcd driver can stabalize
    GPIO_ResetBits(LCD_PORT, LCD_EN);
}

static void write_byte(uint8_t byte){
    uint8_t nibble;
    // Higher nibble
    nibble = byte >> 4;
    write_nibble(nibble);
    // Lower nibble
    nibble = (byte & 0x0F);
    write_nibble(nibble);
}

void lcd_write_cmd(uint8_t cmd){
    GPIO_ResetBits(LCD_PORT, LCD_RS);
    write_byte(cmd);
}

void lcd_write_char(char c){
    GPIO_SetBits(LCD_PORT, LCD_RS);
    write_byte(c);
}

static void update_cursor_pos(void){
    if(cursor_pos > 0x4F){  // Cursor is beyond the 16 columns of the 2nd row
        cursor_pos = 0x00;
    }
    if((cursor_pos > 0x0F) && (cursor_pos < 0x40)){ // Cursor is beyond the 16 columns of the 1st row
        cursor_pos = 0x40;
    }
}

static uint8_t get_cursor_address(uint8_t row, uint16_t column){
    uint8_t cursor_address;
    cursor_address = row * 0x40 + column;
    return cursor_address;
}

void lcd_cursor_put(uint8_t row, uint8_t column){
    uint8_t cursor_target;
    cursor_target = get_cursor_address(row, column);
    cursor_pos = cursor_target;
    update_cursor_pos();
    uint8_t cmd = 0x80 + cursor_pos;
    lcd_write_cmd(cmd);
}

void lcd_clear(void){
    lcd_write_cmd(LCD_CLR);
    lcd_cursor_put(0, 0);
}

static void line_feed(){
    if(cursor_pos & 0xf0){  // If cursor is in 2nd row
        lcd_cursor_put(0, cursor_pos & 0x0f); // Put in 1st row, current column
    }
    else{
        lcd_cursor_put(1, cursor_pos & 0x0f); // Put in 1st row, current column
    }
}

static void carriage_return(){
    if(cursor_pos & 0xf0){  // If cursor is in 2nd row
        lcd_cursor_put(1, 0); // Put in 1st row, 1st column
    }
    else{
        lcd_cursor_put(0, 0); // Put in 1st row, 1st column
    }
}

void lcd_put(char c){
    switch(c){
        case 0: lcd_write_char(0xDB); //Write NULL Char
                break;
        case 10: line_feed();
                 break;
        case 12: line_feed();
                 carriage_return();
                 break;
        case 13: carriage_return();
                 break;
        default: lcd_write_char(c);
    }
}

#include "lcd_driver.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "bsp.h"

#define ALL_DATA_PINS ((uint16_t)(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7))

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

    // Set all data pins to low
    GPIO_ResetBits(LCD_PORT, ALL_DATA_PINS);
    // Set RS to command
    GPIO_ResetBits(LCD_PORT, LCD_RS);
    // Set EN to 1
    GPIO_SetBits(LCD_PORT, LCD_EN);
    uint8_t i;
    // LCD_SHIFT reflects to offset of the controll pins from the 0 pin
    uint16_t port_value = LCD_8b << LCD_SHIFT; 
    // Write 8-bit command 3 times
    for(i = 0; i < 3; i++){
        GPIO_Write(LCD_PORT, port_value);
    }
    // Write 4-bit command
    port_value = LCD_4b << LCD_SHIFT;
    GPIO_Write(LCD_PORT, port_value);
    // Write display mode command TODO: Replace later on with write command
    // High nibble first 
    port_value = (LCD_DM >> 4);
    port_value = port_value << LCD_SHIFT;
    GPIO_Write(LCD_PORT, port_value);
    // Low nibble second
    port_value = (LCD_DM & 0x0f);
    port_value = port_value << LCD_SHIFT;
    GPIO_Write(LCD_PORT, port_value);
    // Send active display command
    port_value = LCD_ACT << LCD_SHIFT;
    GPIO_Write(LCD_PORT, port_value);
    // Clear LCD
    port_value = LCD_CLR << LCD_SHIFT;
    GPIO_Write(LCD_PORT, port_value);
}

static void write_nibble(uint8_t nibble){
    uint16_t port_value;
    port_value = ((uint16_t) nibble) << LCD_SHIFT;
    GPIO_Write(LCD_PORT, port_value);
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

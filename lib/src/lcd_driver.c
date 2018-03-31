#include "lcd_driver.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "bsp.h"

#define ALL_DATA_PINS ((uint16_t)(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7))
#define CHECK_BIT(byte, bit) ((byte) & (1 << (bit)))

static void write_nibble(uint8_t nibble);
static void write_byte(uint8_t byte);

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

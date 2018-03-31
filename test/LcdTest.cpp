#include "TestHarness.h"
#include "MockSupport.h"
#include "MockSupport_c.h"

extern "C"{
    #include <stdio.h>
    #include "stm32f4xx_conf.h"
    #include "stm32f4xx_rcc.h"
    #include "stm32f4xx_gpio.h"
    #include "bsp.h"
    #include "lcd_driver.h"
}

#include "test_gpio_mock.h"
#include "test_rcc_mock.h"

#define CHECK_BIT(byte, bit) ((byte) & (1 << (bit)))

GPIO_InitTypeDef LCD_Init_Def;
uint16_t all_data_pins = LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7;

TEST_GROUP(LcdDriverTestGroup){

    void setup(void){
        // Turn off memory leak detection:
        MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
        LCD_Init_Def.GPIO_Pin = (LCD_RS | LCD_EN | all_data_pins);
        LCD_Init_Def.GPIO_Mode = GPIO_Mode_OUT;
        LCD_Init_Def.GPIO_OType = GPIO_OType_PP;
        LCD_Init_Def.GPIO_PuPd = GPIO_PuPd_NOPULL;
        LCD_Init_Def.GPIO_Speed = GPIO_Speed_50MHz;
    }

    void teardown(void){
        mock().clear();
        MemoryLeakWarningPlugin::turnOnNewDeleteOverloads();
    }
};

void mock_nibble_write(uint8_t nibble){
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LCD_PORT)
                                        .withParameter("GPIO_Pin", LCD_EN);
    for(uint8_t i = 0;i < 4;i++){
        uint16_t pin = 1 << (LCD_SHIFT + i);
        if(CHECK_BIT(nibble, i)){
            mock().expectOneCall("GPIO_SetBits").withParameter("Port", LCD_PORT)
                                                .withParameter("GPIO_Pin", pin);
        }
        else{
            mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                                .withParameter("GPIO_Pin", pin);
        }
    }
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_EN);
}

TEST(LcdDriverTestGroup, InitLcdProperly){
    mock_c()->installComparator("GPIO_InitType", gpio_is_equal, gpio_to_string);
    mock().expectOneCall("RCC_AHB1PeriphClockCmd").withParameter("HW_Clock", RCC_AHB1Periph_GPIOD)
                                                  .withParameter("STATE", ENABLE);
    mock().expectOneCall("GPIO_Init").withParameter("Port", LCD_PORT)
                                    .withParameterOfType("GPIO_InitType",
                                                         "Pin_Init_Struct",
                                                         &LCD_Init_Def);
    // Clear all data bits
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", all_data_pins);
    // Set RS to command 
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    // Enable LCD
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                        .withParameter("GPIO_Pin", LCD_EN);
    // Send 8-bit mode 3 times
    for(uint8_t i = 0;i < 3; i++){
        mock_nibble_write(LCD_8b);
    }
    // Send 4-bit mode command
    mock_nibble_write(LCD_4b);
    // Send display mode command
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    // High nibble first
    mock_nibble_write(0x2);
    // Low nibble second
    mock_nibble_write(0x8);
    // Send active display command
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    // High nibble first
    mock_nibble_write(0x0);
    // Low nibble second
    mock_nibble_write(0xF);
    // Clear LCD
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    // High nibble first
    mock_nibble_write(0x0);
    // Low nibble second
    mock_nibble_write(0x1);

    lcd_init();
    mock().checkExpectations();
}

TEST(LcdDriverTestGroup, LcdSendCommandOneNibble){
    // Set RS to command
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    mock_nibble_write(0x0);
    mock_nibble_write(0xF);
    lcd_write_cmd(LCD_ACT);
    mock().checkExpectations();
}

TEST(LcdDriverTestGroup, LcdSendCommandTwoNibble){
    // Set RS to command
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    mock_nibble_write(0x2);
    mock_nibble_write(0x8);

    lcd_write_cmd(LCD_DM);
    mock().checkExpectations();
}

TEST(LcdDriverTestGroup, LcdSendChar){
    // Set RS to data
    mock().expectOneCall("GPIO_SetBits").withParameter("Port", LCD_PORT)
                                        .withParameter("GPIO_Pin", LCD_RS);
    mock_nibble_write(0x6);
    mock_nibble_write(0x1);

    lcd_write_char('a');
    mock().checkExpectations();
}

TEST(LcdDriverTestGroup, LcdPutCursorR1C1){
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    mock_nibble_write(0x8);
    mock_nibble_write(0x0);

    lcd_cursor_put(0, 0);
    mock().checkExpectations();
}

TEST(LcdDriverTestGroup, LcdPutCursorR2C16){
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    mock_nibble_write(0xC);
    mock_nibble_write(0xF);

    lcd_cursor_put(1, 15);
    mock().checkExpectations();
}

TEST(LcdDriverTestGroup, LcdPutCursorR2C17){
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    mock_nibble_write(0x8);
    mock_nibble_write(0x0);

    lcd_cursor_put(1, 16);
    mock().checkExpectations();
}

TEST(LcdDriverTestGroup, LcdPutCursorR1C17){
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    mock_nibble_write(0xC);
    mock_nibble_write(0x0);

    lcd_cursor_put(0, 16);
    mock().checkExpectations();
}

TEST(LcdDriverTestGroup, LcdClearScreen){
    // Clear screen
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    mock_nibble_write(0x0);
    mock_nibble_write(0x1);
    // Set cursor to 0,0
    mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                          .withParameter("GPIO_Pin", LCD_RS);
    mock_nibble_write(0x8);
    mock_nibble_write(0x0);

    lcd_clear();
    mock().checkExpectations();
}

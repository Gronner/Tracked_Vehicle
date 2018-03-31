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
   // Set RW to write
   mock().expectOneCall("GPIO_ResetBits").withParameter("Port", LCD_PORT)
                                         .withParameter("GPIO_Pin", LCD_RS);
   // Enable LCD
   mock().expectOneCall("GPIO_SetBits").withParameter("Port", LCD_PORT)
                                       .withParameter("GPIO_Pin", LCD_EN);
   // Send 8-bit mode 3 times
   uint16_t port_value = LCD_8b << LCD_SHIFT;
   mock().expectOneCall("GPIO_Write").withParameter("Port", LCD_PORT)
                                     .withParameter("Port_Value", port_value);
   mock().expectOneCall("GPIO_Write").withParameter("Port", LCD_PORT)
                                     .withParameter("Port_Value", port_value);
   mock().expectOneCall("GPIO_Write").withParameter("Port", LCD_PORT)
                                     .withParameter("Port_Value", port_value);
   // Send 4-bit mode command
   port_value = LCD_4b << LCD_SHIFT;
   mock().expectOneCall("GPIO_Write").withParameter("Port", LCD_PORT)
                                     .withParameter("Port_Value", port_value);
   // Send display mode command
   // High nibble first
   port_value = (LCD_DM >> 4);
   port_value = port_value << LCD_SHIFT;
   mock().expectOneCall("GPIO_Write").withParameter("Port", LCD_PORT)
                                     .withParameter("Port_Value", port_value);
   // Low nibble second
   port_value = (LCD_DM & 0x0f);
   port_value = port_value << LCD_SHIFT;
   mock().expectOneCall("GPIO_Write").withParameter("Port", LCD_PORT)
                                     .withParameter("Port_Value", port_value);
   // Send active display command
   port_value = LCD_ACT << LCD_SHIFT;
   mock().expectOneCall("GPIO_Write").withParameter("Port", LCD_PORT)
                                     .withParameter("Port_Value", port_value);
   // Clear LCD
   port_value = LCD_CLR << LCD_SHIFT;
   mock().expectOneCall("GPIO_Write").withParameter("Port", LCD_PORT)
                                     .withParameter("Port_Value", port_value);

   lcd_init();
   mock().checkExpectations();
}

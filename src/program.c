#include <stdint.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

int main(void);
void vApplicationMallocFailedHook (void);
void vApplicationStackOverflowHook(void);

int main(void){
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    __asm("dsb");
    GPIOD->MODER |= (1 << (GPIO_PinSource14 * 2));
    uint32_t i = 0;
    for(;;){
        GPIOD->ODR ^= (1 << GPIO_PinSource14);
        for(i = 0; i<UINT16_MAX*5; i++){}
    }
}

void vApplicationMallocFailedHook(void)
{
}

void vApplicationStackOverflowHook(void)
{
}

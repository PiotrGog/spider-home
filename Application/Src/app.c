#include <Application/app.h>
#include <Application/logger.h>

#include <main.h>

#include <stm32l4xx_hal.h>
#include <stm32l476xx.h>

#include <stdint.h>
#include <stdio.h>

void turnOnRedLed();
void turnOffRedLed();

void sleepForMs(uint32_t delay);

void run()
{
    int loopCount = 0;
    while (1)
    {
        info("Test log. Loop: %d", loopCount);
        turnOnRedLed();
        sleepForMs(100);
        turnOffRedLed();
        sleepForMs(300);
        loopCount++;
    }
}

void turnOnRedLed()
{
    HAL_GPIO_WritePin(GPIOB, LD_R_Pin, GPIO_PIN_SET);
}

void turnOffRedLed()
{
    HAL_GPIO_WritePin(GPIOB, LD_R_Pin, GPIO_PIN_RESET);
}

void sleepForMs(uint32_t delay)
{
    HAL_Delay(delay);
}

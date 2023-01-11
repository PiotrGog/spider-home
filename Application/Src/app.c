#include <Application/app.h>

#include "main.h"

#include <stm32l476xx.h>

#include <stdint.h>

void turnOnRedLed();
void turnOffRedLed();

void sleepForMs(uint32_t delay);

void run()
{
    while (1)
    {
        turnOnRedLed();
        sleepForMs(100);
        turnOffRedLed();
        sleepForMs(100);
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

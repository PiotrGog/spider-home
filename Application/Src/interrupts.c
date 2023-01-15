#include "interrupts.h"

#include <gpio.h>

#include <stdint.h>

uint8_t joyCenterInterruptOccured = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == JOY_CENTER_Pin)
    {
        joyCenterInterruptOccured = 1;
    }
}

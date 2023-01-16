#include "interrupts.h"

#include <gpio.h>

#include <stdint.h>

uint8_t joyCenterInterruptOccured = 0;

extern void SystemClock_Config(void);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    SystemClock_Config();
    HAL_ResumeTick();
    if (GPIO_Pin == JOY_CENTER_Pin)
    {
        joyCenterInterruptOccured = 1;
    }
}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    SystemClock_Config();
    HAL_ResumeTick();
    HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
}

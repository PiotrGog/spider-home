#include "interrupts.h"

#include <Application/app.h>
#include <Application/logger.h>

#include <dht/dht11.h>

#include <tim.h>

#include <stm32l476g_discovery_glass_lcd.h>

#include <stm32l4xx_hal.h>
#include <stm32l476xx.h>

#include <stdint.h>
#include <stdio.h>

#define MESSAGE_BUFFER_SIZE 100

void readAndPrintTemperatureAndHumidity(Dht11 *dht);

void run()
{
    BSP_LCD_GLASS_Init();
    HAL_TIM_Base_Start(&htim2);

    Dht11 dht = dhtConstruct(DHT_GPIO_Port, DHT_Pin, &htim2);

    while (1)
    {
        if (joyCenterInterruptOccured == 1)
        {
            readAndPrintTemperatureAndHumidity(&dht);
            joyCenterInterruptOccured = 0;
        }
        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
        HAL_Delay(2000);
    }
}

void readAndPrintTemperatureAndHumidity(Dht11 *dht)
{
    const uint16_t repeatMessageScroll = 3;
    const uint16_t scrollSpeed = SCROLL_SPEED_MEDIUM;

    DhtData dhtData = {0, 0};
    switch (dhtRead(dht, &dhtData))
    {
    case DhtResult_ResponseFail:
        warning("DhtResult_ResponseFail: connection with DHT11 failed");
        BSP_LCD_GLASS_ScrollSentence(
            (uint8_t *)"          DhtResult_ResponseFail",
            repeatMessageScroll,
            scrollSpeed);
        break;
    case DhtResult_DataReadFail:
        warning("DhtResult_DataReadFail: reading data from DHT11 failed");
        BSP_LCD_GLASS_ScrollSentence(
            (uint8_t *)"          DhtResult_DataReadFail",
            repeatMessageScroll,
            scrollSpeed);
        break;
    case DhtResult_CheckSumFail:
        warning("DhtResult_CheckSumFail: incorrect data received from DHT11");
        BSP_LCD_GLASS_ScrollSentence(
            (uint8_t *)"          DhtResult_CheckSumFail",
            repeatMessageScroll,
            scrollSpeed);
        break;
    case DhtResult_Ok:
        info("Temperature: %d, huminity: %d", dhtData.temperature, dhtData.humidity);
        char buffer[MESSAGE_BUFFER_SIZE];
        sprintf(buffer, "          TEMP %dC, HUM %dRH", dhtData.temperature, dhtData.humidity);
        BSP_LCD_GLASS_ScrollSentence(
            (uint8_t *)buffer,
            repeatMessageScroll,
            scrollSpeed);
        break;
    }
}

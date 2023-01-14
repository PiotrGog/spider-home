#include <Application/app.h>
#include <Application/logger.h>

#include <dht/dht11.h>

#include <tim.h>

#include <stm32l4xx_hal.h>
#include <stm32l476xx.h>

#include <stdint.h>
#include <stdio.h>

void turnOnRedLed();
void turnOffRedLed();

void sleepForMs(uint32_t delay);

void run()
{
    HAL_TIM_Base_Start(&htim2);
    Dht11 dht = dhtConstruct(DHT_GPIO_Port, DHT_Pin, &htim2);
    DhtData dhtData = {0, 0};
    while (1)
    {
        const DhtResult dhtResult = dhtRead(&dht, &dhtData);
        switch (dhtResult)
        {
        case DhtResult_ResponseFail:
            warning("DhtResult_ResponseFail: connection with DHT11 failed");
            break;
        case DhtResult_DataReadFail:
            warning("DhtResult_DataReadFail: reading data from DHT11 failed");
            break;
        case DhtResult_CheckSumFail:
            warning("DhtResult_CheckSumFail: incorrect data received from DHT11");
            break;
        case DhtResult_Ok:
            info("Temperature: %d, huminity: %d", dhtData.temperature, dhtData.humidity);
            break;
        }

        turnOnRedLed();
        sleepForMs(500);
        turnOffRedLed();
        sleepForMs(500);
    }
}

void turnOnRedLed()
{
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
}

void turnOffRedLed()
{
    HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
}

void sleepForMs(uint32_t delay)
{
    HAL_Delay(delay);
}

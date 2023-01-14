#include "dht11_internal.h"

#include <Application/logger.h>

void sendStartSignal(const Dht11 *dht)
{
    debug("DHT sending start signal");

    static const uint32_t startSignalLowStateDelayMSec = 18;

    setToOutputMode(dht);
    HAL_GPIO_WritePin(dht->port, dht->pin, GPIO_PIN_RESET);
    HAL_Delay(startSignalLowStateDelayMSec);
}

DhtResult readResponse(const Dht11 *dht)
{
    setToInputMode(dht);

    const uint32_t maxRetries = 500;
    if (waitForInputState(dht, GPIO_PIN_RESET, maxRetries) >= maxRetries)
    {
        return DhtResult_ResponseFail;
    }
    if (waitForInputState(dht, GPIO_PIN_SET, maxRetries) >= maxRetries)
    {
        return DhtResult_ResponseFail;
    }
    if (waitForInputState(dht, GPIO_PIN_RESET, maxRetries) >= maxRetries)
    {
        return DhtResult_ResponseFail;
    }

    return DhtResult_Ok;
}

DhtResult readSensorData(const Dht11 *dht, uint8_t *dataBuffer, const uint8_t dataSize)
{
    setToInputMode(dht);

    for (uint8_t byteNumber = 0; byteNumber < dataSize; ++byteNumber)
    {
        const DhtResult result = readByte(dht, &dataBuffer[byteNumber]);
        if (result != DhtResult_Ok)
        {
            return result;
        }
    }

    return DhtResult_Ok;
}

DhtResult readByte(const Dht11 *dht, uint8_t *byte)
{
    const uint32_t maxRetries = 500;
    static const uint32_t timeThresholdInHighStateAsOneUSec = 50;
    static const uint8_t bitsInByte = 8;
    for (uint8_t bitNumber = 0; bitNumber < bitsInByte; ++bitNumber)
    {
        if (waitForInputState(dht, GPIO_PIN_SET, maxRetries) >= maxRetries)
        {
            return DhtResult_DataReadFail;
        }
        if (waitForInputState(dht, GPIO_PIN_RESET, maxRetries) >= maxRetries)
        {
            return DhtResult_DataReadFail;
        }
        const uint32_t timeInUpStateUSec = __HAL_TIM_GET_COUNTER(&htim2);
        *byte <<= 1;
        if (timeInUpStateUSec > timeThresholdInHighStateAsOneUSec)
        {
            *byte |= 1;
        }
    }

    return DhtResult_Ok;
}

DhtResult verifyData(uint8_t *dataBuffer, const uint8_t dataSize)
{
    uint8_t sum = 0;
    for (uint8_t index = 0; index < dataSize - 1; ++index)
    {
        sum += dataBuffer[index];
    }

    if (sum != dataBuffer[dataSize - 1])
    {
        return DhtResult_CheckSumFail;
    }
    return DhtResult_Ok;
}

uint32_t waitForInputState(const Dht11 *dht, const GPIO_PinState pinState, const uint32_t timeout)
{
    __HAL_TIM_SET_COUNTER(dht->timer, 0);
    while (HAL_GPIO_ReadPin(dht->port, dht->pin) != pinState && __HAL_TIM_GET_COUNTER(&htim2) < timeout)
    {
    }
    return __HAL_TIM_GET_COUNTER(dht->timer);
}

void setToOutputMode(const Dht11 *dht)
{
    GPIO_InitTypeDef newGpioSettings = {0};
    newGpioSettings.Pin = dht->pin;
    newGpioSettings.Mode = GPIO_MODE_OUTPUT_PP;
    newGpioSettings.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    newGpioSettings.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(dht->port, &newGpioSettings);
}

void setToInputMode(const Dht11 *dht)
{
    GPIO_InitTypeDef newGpioSettings = {0};
    newGpioSettings.Pin = dht->pin;
    newGpioSettings.Mode = GPIO_MODE_INPUT;
    newGpioSettings.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    newGpioSettings.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(dht->port, &newGpioSettings);
}

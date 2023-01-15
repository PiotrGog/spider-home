#include "dht11_internal.h"

#include <Application/logger.h>

#include <dht/dht11.h>

#define DKT_DATA_BUFFER_SIZE 5

Dht11 dhtConstruct(GPIO_TypeDef *port, uint16_t pin, TIM_HandleTypeDef *timer)
{
    info("Creating DHT11 instance");
    Dht11 dht = {port, pin, timer};
    return dht;
}

DhtResult dhtRead(const Dht11 *dht, DhtData *result)
{
    info("Reading DHT11 data");

    sendStartSignal(dht);
    const DhtResult readResponseResult = readResponse(dht);
    if (readResponseResult != DhtResult_Ok)
    {
        return readResponseResult;
    }

    uint8_t dataBuffer[DKT_DATA_BUFFER_SIZE] = {0, 0, 0, 0, 0};
    const DhtResult readSensorDataResult = readSensorData(dht, dataBuffer, DKT_DATA_BUFFER_SIZE);
    if (readSensorDataResult != DhtResult_Ok)
    {
        return readSensorDataResult;
    }

    const DhtResult verifyDataResult = verifyData(dataBuffer, DKT_DATA_BUFFER_SIZE);
    if (verifyDataResult != DhtResult_Ok)
    {
        return verifyDataResult;
    }

    const uint8_t humidityDataIndex = 0;
    const uint8_t temperatureDataIndex = 2;
    result->humidity = dataBuffer[humidityDataIndex];
    result->temperature = dataBuffer[temperatureDataIndex];

    return DhtResult_Ok;
}

#include "dht11_internal.h"

#include <Application/logger.h>

#include <dht/dht11.h>

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

    uint8_t dataBuffer[5] = {0, 0, 0, 0, 0};
    const DhtResult readSensorDataResult = readSensorData(dht, dataBuffer, 5);
    if (readSensorDataResult != DhtResult_Ok)
    {
        return readSensorDataResult;
    }

    const DhtResult verifyDataResult = verifyData(dataBuffer, 5);
    if (verifyDataResult != DhtResult_Ok)
    {
        return verifyDataResult;
    }

    result->humidity = dataBuffer[0];
    result->temperature = dataBuffer[2];

    return DhtResult_Ok;
}

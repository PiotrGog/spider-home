#ifndef __DHT11_H__
#define __DHT11_H__

#include <gpio.h>
#include <tim.h>

#include <stdint.h>

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    TIM_HandleTypeDef *timer;
} Dht11;

typedef struct
{
    uint8_t temperature;
    uint8_t humidity;
} DhtData;

typedef enum
{
    DhtResult_Ok,
    DhtResult_CheckSumFail,
    DhtResult_ResponseFail,
    DhtResult_DataReadFail
} DhtResult;

Dht11 dhtConstruct(GPIO_TypeDef *port, uint16_t pin, TIM_HandleTypeDef *timer);

DhtResult dhtRead(const Dht11 *dht, DhtData *result);

#endif // __DHT11_H__

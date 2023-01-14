#ifndef __DHT11_INTERNAL_H__
#define __DHT11_INTERNAL_H__

#include <dht/dht11.h>

#include <tim.h>
#include <gpio.h>

#include <stdint.h>

void sendStartSignal(const Dht11 *dht);

DhtResult readResponse(const Dht11 *dht);

DhtResult readSensorData(const Dht11 *dht, uint8_t *dataBuffer, const uint8_t dataSize);

DhtResult readByte(const Dht11 *dht, uint8_t *byte);

DhtResult verifyData(uint8_t *dataBuffer, const uint8_t dataSize);

uint32_t waitForInputState(const Dht11 *dht, const GPIO_PinState pinState, const uint32_t timeout);

void setToOutputMode(const Dht11 *dht);

void setToInputMode(const Dht11 *dht);

#endif // __DHT11_INTERNAL_H__

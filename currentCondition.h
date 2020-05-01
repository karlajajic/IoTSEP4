#pragma once
#include <stdint.h>

typedef struct currentCondition* currentCondition_t;

currentCondition_t currentCondition_create(int deviceId, uint16_t co2Data, uint16_t temperatureData, uint16_t humidityData, uint16_t soundData);
void currentCondition_destroy(currentCondition_t self);
//toString????
#pragma once
#include <stdint.h>

typedef struct currentCondition* currentCondition_t;

<<<<<<< HEAD
currentCondition_t currentCondition_create(int deviceId);

void currentCondition_setCO2(currentCondition_t self, uint16_t value);
void currentCondition_setHumidity(currentCondition_t self, uint16_t value);
void currentCondition_setTemperature(currentCondition_t self, uint16_t value);
void currentCondition_setSound(currentCondition_t self, uint16_t value);

void currentCondition_destroy(currentCondition_t self);
=======
currentCondition_t currentCondition_create(int deviceId, uint16_t co2Data, uint16_t temperatureData, uint16_t humidityData, uint16_t soundData);
void currentCondition_destroy(currentCondition_t self);
//toString????
>>>>>>> parent of 32d18b3... changed design

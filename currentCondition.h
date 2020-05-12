#pragma once
#include <stdint.h>

typedef struct currentCondition* currentCondition_t;

currentCondition_t currentCondition_create(int deviceId);

void currentCOndition_setCO2(currentCondition_t self, uint16_t value);
void currentCOndition_setHumidity(currentCondition_t self, uint16_t value);
void currentCOndition_setTemperature(currentCondition_t self, uint16_t value);
void currentCOndition_setSound(currentCondition_t self, uint16_t value);

void currentCondition_destroy(currentCondition_t self);
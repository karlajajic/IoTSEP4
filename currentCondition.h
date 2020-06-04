#pragma once
#include <stdint.h>
#include <lora_driver.h>

typedef struct currentCondition* currentCondition_t;

currentCondition_t currentCondition_create();

void currentCondition_setCO2(currentCondition_t self, uint16_t value);
void currentCondition_setHumidity(currentCondition_t self, uint16_t value);
void currentCondition_setTemperature(currentCondition_t self, int16_t value);
void currentCondition_setSound(currentCondition_t self, uint16_t value);

void currentCondition_destroy(currentCondition_t self);
lora_payload_t getcurrentConditionPayload(currentCondition_t self);
lora_payload_t getSimplePayload(currentCondition_t self);
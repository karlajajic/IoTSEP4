#pragma once
#include <stdint.h>
#include <lora_driver.h>// the lora_driver.h include should be commented in order the tests to work

typedef struct currentCondition* currentCondition_t;

currentCondition_t currentCondition_create();

void currentCondition_setCO2(currentCondition_t self, uint16_t value);
void currentCondition_setHumidity(currentCondition_t self, uint16_t value);
void currentCondition_setTemperature(currentCondition_t self, int16_t value);
void currentCondition_setSound(currentCondition_t self, uint16_t value);

void currentCondition_destroy(currentCondition_t self);

// The following for methods were implemented so the code can be tested 
uint16_t currentCondition_getCO2Data(currentCondition_t self);
int16_t currentCondition_getTemperature(currentCondition_t self);
uint16_t currentCondition_getHumidity(currentCondition_t self);
uint16_t currentCondition_getSound(currentCondition_t self);


// The methods below should be commented out in order for the test to work
lora_payload_t getcurrentConditionPayload(currentCondition_t self);
lora_payload_t getSimplePayload(currentCondition_t self);
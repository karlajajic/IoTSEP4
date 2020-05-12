#pragma once
#include "currentCondition.h"
#include "DeviceConfig.h"

typedef struct device* device_t;

device_t device_create(void);
void device_destroy(device_t self);
void device_executeTask(device_t self);
void device_startMeasuring(device_t self);

currentCondition_t device_getCurrentCondition(device_t self);

void device_setCO2ToCurrent(device_t self, uint16_t value);


uint16_t device_getCO2Data(device_t self);

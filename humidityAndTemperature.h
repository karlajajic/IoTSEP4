#pragma once

#include <stdint.h>
#include <stdlib.h>
#include "hih8120.h"

#include <ATMEGA_FreeRTOS.h>
#include "task.h"
#include "event_groups.h"

typedef struct humidityAndTemperature* humAndTempReader_t;

humAndTempReader_t humAndTempReader_create(TaskHandle_t taskHandle, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit);
void humAndTempReader_destroy(humAndTempReader_t self);

void humAndTempReader_executeTask(void* self);
void humAndTempReader_measure(humAndTempReader_t self);
uint16_t humAndTempReader_getHumidity(humAndTempReader_t self);
int16_t humAndTempReader_getTemperature(humAndTempReader_t self);

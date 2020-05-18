#pragma once

#include <stdint.h>
#include <stdlib.h>

#include <ATMEGA_FreeRTOS.h>
#include "task.h"
#include "event_groups.h"

typedef struct humidityAndTemperature* humAndTempReader_t;

humAndTempReader_t humAndTempReader_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit);
void humAndTempReader_destroy(humAndTempReader_t self);

void humAndTempReader_executeTask(humAndTempReader_t self);
void humAndTempReader_measure(humAndTempReader_t self);
float humAndTempReader_getHumidity(humAndTempReader_t self);
float humAndTempReader_getTemperature(humAndTempReader_t self);

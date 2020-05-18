#pragma once
#include <stdint.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

typedef struct co2reader* co2reader_t;

co2reader_t co2Reader_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit);
void co2Reader_destroy(co2reader_t self);

void co2Reader_executeTask(co2reader_t self);
void co2Reader_measure(co2reader_t self);
uint8_t co2Reader_getCO2(co2reader_t self);
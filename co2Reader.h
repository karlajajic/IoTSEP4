#pragma once
#include <stdint.h>
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

typedef struct co2reader* co2reader_t;

co2reader_t co2Reader_create(TaskHandle_t taskHadnle,EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit);
void co2Reader_destroy(co2reader_t self);

void co2Reader_executeTask(void* self);
void co2Reader_measure(co2reader_t self);
uint16_t co2Reader_getCO2(co2reader_t self);
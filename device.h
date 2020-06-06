#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <event_groups.h>
#include <message_buffer.h>
#include <hal_defs.h>

#include "currentCondition.h"
#include "co2Reader.h"
#include "humidityAndTemperature.h"
#include "SoundReader.h"
#include "event_groups.h"
#include <stdint.h>

typedef struct device* device_t;

device_t device_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit, co2reader_t co2Reader, humAndTempReader_t humAndTempReader,soundReader_t soundReader, MessageBufferHandle_t uplinkMessageBuffer, EventGroupHandle_t waitEventGroup, EventBits_t deviceBit);
void device_executeTask(void* self);
void device_startMeasuring(device_t self);

currentCondition_t device_getCurrentCondition(device_t self);

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
#include <stdint.h>

typedef struct device* device_t;

device_t device_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit, co2reader_t co2Reader, humAndTempReader_t humAndTempReader,soundReader_t soundReader, MessageBufferHandle_t uplinkMessageBuffer);
//void device_destroy(device_t self);
void device_executeTask(void* self);
void device_startMeasuring(device_t self);

currentCondition_t device_getCurrentCondition(device_t self);

void device_setHumidityToCurrent(device_t self, uint16_t value);


uint16_t device_getHumidityData(device_t self);

void device_setCO2ToCurrent(device_t self, uint16_t value);


uint16_t device_getCO2Data(device_t self);

void device_setSoundToCurrent(device_t self, uint16_t value);

uint16_t device_getSoundData(device_t self);

void device_setTemperatureToCurrent(device_t self, int16_t value);


int16_t device_getTemperatureData(device_t self);
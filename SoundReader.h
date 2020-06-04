
/*
 * SoundReader.h
 *
 * Created: 3.6.2020 г. 18:05:26 ч.
 *  Author: user
 */ 
#pragma once

#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include "task.h"
#include "event_groups.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct soundReader* soundReader_t;

soundReader_t soundReader_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit);
void soundReader_destroy(soundReader_t self);
void soundReader_executeTask(void* self);
void soundReader_measure(soundReader_t self);
uint16_t soundReader_getSound(soundReader_t self);

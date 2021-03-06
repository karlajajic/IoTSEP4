﻿#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include "task.h"
#include "event_groups.h"
#include "SoundReader.h"

static EventGroupHandle_t _startMeasureEventGroup;
static EventBits_t _startMeasureBit;

static EventGroupHandle_t _readyMeasuringEventGroup;
static EventBits_t _readyBit;

typedef struct soundReader soundReader;


typedef struct soundReader {
	uint16_t value;
	TaskHandle_t handleTask;
}soundReader;


//actual task, methods devided so that it is possible to test
void soundReader_executeTask(void* self) {
	for (;;) {
		soundReader_measure((soundReader_t)self);
	}
}

soundReader_t soundReader_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit) {

	soundReader_t _new_reader = calloc(1, sizeof(soundReader));
	if (_new_reader == NULL)
	return NULL;

	_new_reader->value = 0;

	_startMeasureEventGroup = startMeasureEventGroup;
	_startMeasureBit = startMeasureBit;

	_readyMeasuringEventGroup = readyEventGroup;
	_readyBit = readyBit;

	xTaskCreate(
	soundReader_executeTask,
	"soundReader",
	stack + 200,
	_new_reader,
	priority,
	&_new_reader->handleTask
	);

	return _new_reader;
}

void soundReader_destroy(soundReader_t self) {
	if (self == NULL)
		return;

	vTaskDelete(self->handleTask);
	vPortFree(self);
}


void soundReader_measure(soundReader_t self) {
	
	EventBits_t uxBits = xEventGroupWaitBits(_startMeasureEventGroup, 
	_startMeasureBit, 
	pdTRUE, 
	pdTRUE, 
	portMAX_DELAY); 

	if ((uxBits & (_startMeasureBit)) == (_startMeasureBit)) {
		srand(time(NULL));
		self->value = rand()%105 + 15;
		
		//set done bit so that device knows measurement is done
		xEventGroupSetBits(_readyMeasuringEventGroup, _readyBit);
	}
}

uint16_t soundReader_getSound(soundReader_t self) {
	return self->value;
}

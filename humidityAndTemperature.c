#include"humidityAndTemperature.h"
#include <stdint.h>
#include <stdlib.h>
#include "hih8120.h"


#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <hal_defs.h>
#include <ihal.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include <FreeRTOSTraceDriver.h>
#include <stdio_driver.h>
#include <serial.h>

static EventGroupHandle_t _startMeasureEventGroup;
static EventBits_t _startMeasureBit;

static EventGroupHandle_t _readyEventGroup;
static EventBits_t _readyBit;

typedef struct humidityAndTemperature humidityAndTemperature;

typedef struct humidityAndTemperature {
	uint16_t humidity;
	int16_t temperature;
	TaskHandle_t handleTask;
}humidityAndTemperature;

void humAndTempReader_executeTask(void* self) {
	for (;;) {
		humAndTempReader_measure((humAndTempReader_t)self);
	}
}

humAndTempReader_t humAndTempReader_create(UBaseType_t priority, UBaseType_t stack, 
EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit) {
	humAndTempReader_t _new_reader = calloc(1, sizeof(humidityAndTemperature));
	if (_new_reader == NULL)
	return NULL;

	_new_reader->humidity = 0;
	_new_reader->temperature = 0;

	_startMeasureEventGroup = startMeasureEventGroup;
	_startMeasureBit = startMeasureBit;

	_readyEventGroup = readyEventGroup;
	_readyBit = readyBit;

	hih8120Create();
	
		xTaskCreate(
		humAndTempReader_executeTask,
		"HumAndTempReader",
		stack + 200,
		_new_reader,
		priority,
		&_new_reader->handleTask
		);
	return _new_reader;
}

void humAndTempReader_destroy(humAndTempReader_t self) {
	if (self == NULL)
		return;
	vTaskDelete(self->handleTask);
	vPortFree(self);
}

void humAndTempReader_measure(humAndTempReader_t self) {
	EventBits_t uxBits = xEventGroupWaitBits(_startMeasureEventGroup, 
	_startMeasureBit, 
	pdTRUE, 
	pdTRUE, 
	portMAX_DELAY);

	if ((uxBits & (_startMeasureBit)) == (_startMeasureBit)) {
		hih8120Wakeup();
		vTaskDelay(50);
		hih8120Meassure();
		vTaskDelay(5);
		if(hih8120IsReady())
		{
			self->humidity = hih8120GetHumidityPercent_x10();
			self->temperature = hih8120GetTemperature_x10();
		}
		vTaskDelay(1000);
		xEventGroupSetBits(_readyEventGroup, _readyBit);
	}
}

uint16_t humAndTempReader_getHumidity(humAndTempReader_t self){
	return self->humidity;
}

int16_t humAndTempReader_getTemperature(humAndTempReader_t self){
	return self->temperature;
}

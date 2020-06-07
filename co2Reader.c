
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include "task.h"
#include "event_groups.h"
#include <mh_z19.h>
#include "co2Reader.h"

static EventGroupHandle_t _startMeasureEventGroup;
static EventBits_t _startMeasureBit;

static EventGroupHandle_t _readyEventGroup;
static EventBits_t _readyBit;

typedef struct co2reader co2reader;

typedef struct co2reader {
	uint16_t value;
	TaskHandle_t handleTask;
}co2reader;

void co2Reader_executeTask(void* self) {
	for (;;) {
		co2Reader_measure((co2reader_t)self);
	}
}

co2reader_t co2Reader_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit) {

	co2reader_t _new_reader = calloc(1, sizeof(co2reader));
	if (_new_reader == NULL)
	return NULL;

	_new_reader->value = 0;

	_startMeasureEventGroup = startMeasureEventGroup;
	_startMeasureBit = startMeasureBit;

	_readyEventGroup = readyEventGroup;
	_readyBit = readyBit;

	mh_z19_create(ser_USART3, NULL); 
	
	xTaskCreate(
	co2Reader_executeTask,
	"CO2Reader",
	stack + 200,
	_new_reader,
	priority,
	&_new_reader->handleTask
	);

	return _new_reader;
}

void co2Reader_destroy(co2reader_t self) {
	if (self == NULL)
		return;
	vTaskDelete(self->handleTask);
	vPortFree(self);
}


void co2Reader_measure(co2reader_t self) {
	
	EventBits_t uxBits = xEventGroupWaitBits(_startMeasureEventGroup,
	_startMeasureBit,
	pdTRUE,
	pdTRUE, 
	portMAX_DELAY);
	
	if ((uxBits & (_startMeasureBit)) == (_startMeasureBit)) {
				
		mh_z19_return_code_t return_code_co2_measurement = mh_z19_take_meassuring();
		vTaskDelay(300);
		if(return_code_co2_measurement == MHZ19_OK) {
			mh_z19_get_co2_ppm(&self->value);
		}
		xEventGroupSetBits(_readyEventGroup, _readyBit);
	}
}

uint16_t co2Reader_getCO2(co2reader_t self) {
	return self->value;
}

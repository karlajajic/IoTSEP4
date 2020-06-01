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

//actual task, methods devided so that it is possible to test
void humAndTempReader_executeTask(void* self) {
	for (;;) {
		humAndTempReader_measure((humAndTempReader_t)self);
	}
}

humAndTempReader_t humAndTempReader_create(TaskHandle_t taskHandle, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit) 
{
	humAndTempReader_t _new_reader = calloc(sizeof(humidityAndTemperature), 1);
	if (_new_reader == NULL)
	return NULL;

	_new_reader->humidity = 0;
	_new_reader->temperature = 0;
	_new_reader->handleTask=taskHandle;

	_startMeasureEventGroup = startMeasureEventGroup;
	_startMeasureBit = startMeasureBit;

	_readyEventGroup = readyEventGroup;
	_readyBit = readyBit;

	printf("humidity and temperature ready\n");
	
	return _new_reader;
}
void humAndTempReader_destroy(humAndTempReader_t self) {
	if (self == NULL)
	return;

	//delete will clear the allocated memory to the task + we need to remove everything else
	vTaskDelete(self->handleTask);

	//free the values from struct (without pointer, later the pointer
	//free(self->handleTask);
	//free(self->humidity);
	//free(self->temperature);
	//free(self);
}



void humAndTempReader_measure(humAndTempReader_t self) {//dummy
	EventBits_t uxBits = xEventGroupWaitBits(_startMeasureEventGroup, //eventGroup
	_startMeasureBit, //bits it is interested i
	pdTRUE, //clears the bits
	pdTRUE, //waits for both bits to be set
	portMAX_DELAY); //wait

	if ((uxBits & (_startMeasureBit)) == (_startMeasureBit)) {
		hih8120Wakeup();
		vTaskDelay(50);
		hih8120Meassure();
		vTaskDelay(5);
		if(hih8120IsReady())
		{
			self->humidity = hih8120GetHumidityPercent_x10();
			self->temperature = hih8120GetTemperature_x10();
			printf("humidity and temperature done bit set\n");
		}
		//set done bit so that device knows meassurement is done
		xEventGroupSetBits(_readyEventGroup, _readyBit);
	}
}

uint16_t humAndTempReader_getHumidity(humAndTempReader_t self){
	return self->humidity;
}

int16_t humAndTempReader_getTemperature(humAndTempReader_t self){
	return self->temperature;
}

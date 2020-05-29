#include"co2Reader.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include "task.h"
#include "event_groups.h"
#include <mh_z19.h>

static EventGroupHandle_t _startMeasureEventGroup;
static EventBits_t _startMeasureBit;

static EventGroupHandle_t _readyEventGroup;
static EventBits_t _readyBit;

typedef struct co2reader co2reader;

static uint16_t ppmValue;

typedef struct co2reader {
	uint16_t value;
	TaskHandle_t handleTask;
}co2reader;

void my_co2_call_back(uint16_t ppm)
{	
	ppmValue = ppm;
	printf("CO2 in callback: %d", ppm);
}

void co2Reader_executeTask(void* self) {
	
	//mh_z19_create(ser_USART3, my_co2_call_back);
	for (;;) {
		co2Reader_measure((co2reader_t)self);
	}
}

co2reader_t co2Reader_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit) {

	co2reader_t _new_reader = calloc(sizeof(co2reader), 1);
	if (_new_reader == NULL)
	return NULL;

	_new_reader->value = 0;

	_startMeasureEventGroup = startMeasureEventGroup;
	_startMeasureBit = startMeasureBit;

	_readyEventGroup = readyEventGroup;
	_readyBit = readyBit;

	mh_z19_create(ser_USART3, my_co2_call_back); 
	
	xTaskCreate(
	co2Reader_executeTask,
	"CO2Reader",
	stack,
	_new_reader,
	priority,
	&_new_reader->handleTask
	);

	printf("co2 up\n");

	return _new_reader;
}



//	RETURN TO DESTROY METHODS
void co2Reader_destroy(co2reader_t self) {
	//if (self == NULL)
	//	return;

	////delete will clear the allocated memory to the task + we need to remove everything else
	//vTaskDelete(self->handleTask);

	////free the values from struct (without pointer, later the pointer
	//free(self->handleTask);
	//free(self->value);
	//free(self);
}

//actual task, methods devided so that it is possible to test


void co2Reader_measure(co2reader_t self) {
	
	EventBits_t uxBits = xEventGroupWaitBits(_startMeasureEventGroup, //eventGroup
	_startMeasureBit, //bits it is interested in
	pdTRUE, //clears the bits
	pdTRUE, //waits for both bits to be set
	portMAX_DELAY); //wait

	if ((uxBits & (_startMeasureBit)) == (_startMeasureBit)) {

		mh_z19_take_meassuring();
		//if(rc1 != MHZ19_OK)
		//{
			//printf("There was a problem with measure co2\n");
		//}

		//uint16_t a = pvPortMalloc(sizeof(uint16_t));
		////uint16_t a;
		//mh_z19_return_code_t rc2 = mh_z19_get_co2_ppm(a);
		//if(rc2 != MHZ19_OK)
		//{
			//printf("There was a problem with get co2\n");
		//}
		//if(rc2 == MHZ19_NO_MEASSURING_AVAILABLE)
		//{
			//printf("There was no co2 measured\n");
		//}
		//my_co2_call_back(a);
		
		self->value = ppmValue;
		//vPortFree(a);

		printf("co2 done bit set\n");
		//set done bit so that device knows measurement is done
		xEventGroupSetBits(_readyEventGroup, _readyBit);
	}
}

uint16_t co2Reader_getCO2(co2reader_t self) {
	return self->value;
}

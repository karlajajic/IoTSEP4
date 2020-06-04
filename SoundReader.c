
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include "task.h"
#include "event_groups.h"
#include "SoundReader.h"

static EventGroupHandle_t _startMeasureEventGroup;
static EventBits_t _startMeasureBit;

static EventGroupHandle_t _readyEventGroup;
static EventBits_t _readyBit;

typedef struct soundReader soundReader;



typedef struct soundReader {
	uint16_t value;
	TaskHandle_t handleTask;
}soundReader;



void soundReader_executeTask(void* self) {
	for (;;) {
		soundReader_measure((soundReader_t)self);
		//vTaskDelay(5000);
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

	_readyEventGroup = readyEventGroup;
	_readyBit = readyBit;

	xTaskCreate(
	soundReader_executeTask,
	"soundReader",
	stack,
	_new_reader,
	priority,
	&_new_reader->handleTask
	);

	printf("soundReader up\n");

	return _new_reader;
}



//	RETURN TO DESTROY METHODS
void soundReader_destroy(soundReader_t self) {
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


void soundReader_measure(soundReader_t self) {
	
	EventBits_t uxBits = xEventGroupWaitBits(_startMeasureEventGroup, //eventGroup
	_startMeasureBit, //bits it is interested in
	pdTRUE, //clears the bits
	pdTRUE, //waits for both bits to be set
	portMAX_DELAY); //wait

	if ((uxBits & (_startMeasureBit)) == (_startMeasureBit)) {

		
		//srand(time(NULL));
		self->value = rand()%105 + 15;
		
		
		//set done bit so that device knows measurement is done
		xEventGroupSetBits(_readyEventGroup, _readyBit);
		printf("SoundReader done bit set\n");
	}
}

uint16_t soundReader_getSound(soundReader_t self) {
	return self->value;
}

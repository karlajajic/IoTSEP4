
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "Configuration.h"
#include <semphr.h>
#include <stdbool.h>

static bool _working;
static bool _ventilation;
static SemaphoreHandle_t _semaphore;


void configuration_create() {
//it is created only once, before starting the scheduler + will never be destroyed
	_working = calloc(1, sizeof(bool));
	_ventilation = calloc(1, sizeof(bool));

	//by default, device should work, not to ventilate
	_working = true;
	_ventilation = false;

	_semaphore = xSemaphoreCreateMutex();
}

//if the semaphore is initialized, take it and give it back, if not, just do the work 
void configuration_setWorking(bool value) {
	if (_semaphore != NULL) {
		xSemaphoreTake(_semaphore, portMAX_DELAY);
		_working = value;
		xSemaphoreGive(_semaphore);
	}
	else _working = value;
}

void configuration_getWorking(bool* working) {
	if (_semaphore != NULL) {
		xSemaphoreTake(_semaphore, portMAX_DELAY);
		*working = _working;
		xSemaphoreGive(_semaphore);
	}
	else *working = _working;
}

void configuration_setVentilation(bool value) {
	if (_semaphore != NULL) {
		xSemaphoreTake(_semaphore, portMAX_DELAY);
		_ventilation = value;
		xSemaphoreGive(_semaphore);
	}
	else _ventilation = value;
}

void configuration_getVentilation(bool* ventilation) {
	if (_semaphore != NULL) {
		xSemaphoreTake(_semaphore, portMAX_DELAY);
		*ventilation = _ventilation;
		xSemaphoreGive(_semaphore);
	}
	else *ventilation = _ventilation;
}
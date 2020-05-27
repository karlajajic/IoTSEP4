/*
 * Configuration.c
 *
 * Created: 14/05/2020 11:32:08
 *  Author: Haralambi & Karla hihihihih
 */ 
#include "ATMEGA_FreeRTOS.h"
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
	_working = calloc(sizeof(bool), 1);
	_ventilation = calloc(sizeof(bool), 1);

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

//Haralambi's part...........

//typedef struct Configuration Configuration;

//typedef struct Configuration
//{
	//bool deviceON;
	//int deviceID;
	//
//}Configuration;
//
//configuration_t configuration_create(bool deviceOn, int deviceId)
//{
	//
	//configuration_t newConfiguration = calloc(sizeof(Configuration), 1);
//
	//if (newConfiguration!=NULL)
	//{
		//newConfiguration->deviceID=deviceId;
		//newConfiguration->deviceON=deviceOn;
		//return newConfiguration;
	//}
	//return NULL;
//}
//bool configuration_getDeviceON(configuration_t configuration, SemaphoreHandle_t mutex)
//{
	//if (xSemaphoreTake(mutex,1000))
	//{
		//return configuration->deviceON;
	//}
	//else
	//{
		//printf("Failed to access deviceON in 1 second");
	//}
	//return false;
//}
//int configuration_getDeviceID(configuration_t configuration, SemaphoreHandle_t mutex)
//{
	//if (xSemaphoreTake(mutex,1000))
	//{
		//return configuration->deviceID;
	//}
	//else
	//{
		//printf("Failed to access deviceID in 1 second");
	//}
	//return -1;
//}
//
//


/*
 * Configuration.c
 *
 * Created: 14/05/2020 11:32:08
 *  Author: Haralambi
 */ 
#include "ATMEGA_FreeRTOS.h"
#include <stdio.h>
#include <stddef.h>
#include "Configuration.h"
#include <semphr.h>
#include <stdbool.h>

//typedef struct Configuration Configuration;

typedef struct Configuration
{
	bool deviceON;
	int deviceID;
	
}Configuration;

configuration_t configuration_create(bool deviceOn, int deviceId)
{
	
	configuration_t newConfiguration = calloc(sizeof(Configuration), 1);

	if (newConfiguration!=NULL)
	{
		newConfiguration->deviceID=deviceId;
		newConfiguration->deviceON=deviceOn;
		return newConfiguration;
	}
	return NULL;
}
bool configuration_getDeviceON(configuration_t configuration, SemaphoreHandle_t mutex)
{
	if (xSemaphoreTake(mutex,1000))
	{
		return configuration->deviceON;
	}
	else
	{
		printf("Failed to access deviceON in 1 second");
	}
	return false;
}
int configuration_getDeviceID(configuration_t configuration, SemaphoreHandle_t mutex)
{
	if (xSemaphoreTake(mutex,1000))
	{
		return configuration->deviceID;
	}
	else
	{
		printf("Failed to access deviceID in 1 second");
	}
	return -1;
}



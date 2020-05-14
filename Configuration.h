
/*
 * Configuration.h
 *
 * Created: 14/05/2020 11:49:12
 *  Author: Haralambi
 */ 
#pragma once
#include "ATMEGA_FreeRTOS.h"
#include <stdbool.h>
#include "semphr.h"

typedef struct Configuration* configuration_t;

configuration_t configuration_create(bool deviceOn, int deviceId);
bool configuration_getDeviceON(configuration_t configuration, SemaphoreHandle_t mutex);
int configuration_getDeviceID(configuration_t configuration, SemaphoreHandle_t mutex);
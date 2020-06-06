#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <stdbool.h>
#include <semphr.h>

void configuration_create(SemaphoreHandle_t semaphore);

void configuration_setWorking(bool value);
void configuration_getWorking(bool* working);

void configuration_setVentilation(bool value);
void configuration_getVentilation(bool* ventilation);

// again, Haralambi's stuff.....

//typedef struct Configuration* configuration_t;
//
//configuration_t configuration_create(bool deviceOn, int deviceId);
//bool configuration_getDeviceON(configuration_t configuration, SemaphoreHandle_t mutex);
//int configuration_getDeviceID(configuration_t configuration, SemaphoreHandle_t mutex);
#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <stdbool.h>
#include <semphr.h>

void configuration_create();

void configuration_setWorking(bool value);
void configuration_getWorking(bool* working);

void configuration_setVentilation(bool value);
void configuration_getVentilation(bool* ventilation);

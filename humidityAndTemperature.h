#pragma once
#include "DeviceConfig.h"

typedef struct humidityAndTemperature* humAndTempReader_t;

humAndTempReader_t humAndTempReader_create(void);
void humAndTempReader_destroy(humAndTempReader_t self);

void humAndTempReader_executeTask(humAndTempReader_t self);
void humAndTempReader_measure(humAndTempReader_t self);
float humAndTempReader_getHumidity(humAndTempReader_t self);
float humAndTempReader_getTemperature(humAndTempReader_t self);

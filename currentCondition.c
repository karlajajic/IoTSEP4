#include <stdlib.h>
#include<stdint.h>
#include "currentCondition.h"

typedef struct currentCondition currentCondtion;

typedef struct currentCondition {
	int deviceId;
	uint16_t co2Data;
	uint16_t temperatureData;
	uint16_t humidityData;
	uint16_t soundData;
}currentCondition;

currentCondition_t currentCondition_create(int deviceId, uint16_t co2Data, uint16_t temperatureData, uint16_t humidityData, uint16_t soundData) {
	currentCondition_t _new_con = calloc(sizeof(currentCondition),1);
	if (_new_con == NULL)
		return NULL;

	_new_con->deviceId = deviceId;
	_new_con->co2Data = co2Data;
	_new_con->temperatureData = temperatureData;
	_new_con->humidityData = humidityData;
	_new_con->soundData = soundData;

	return _new_con;
}

void currentCondition_destroy(currentCondition_t self) {
	if (self == NULL)
		return;
	free(self->deviceId);
	free(self->co2Data);
	free(self->temperatureData);
	free(self->humidityData);
	free(self->soundData);
	free(self);
}
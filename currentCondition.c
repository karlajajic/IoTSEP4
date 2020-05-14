#include <stdlib.h>
#include<stdint.h>
#include "currentCondition.h"

typedef struct currentCondition currentCondtion;

typedef struct currentCondition {
	int deviceId;
	uint16_t co2Data;
	float temperatureData;
	float humidityData;
	uint16_t soundData;
}currentCondition;

currentCondition_t currentCondition_create(int deviceId) {
	currentCondition_t _new_con = calloc(sizeof(currentCondition), 1);
	if (_new_con == NULL)
	return NULL;

	_new_con->deviceId = deviceId;

	return _new_con;
}


void currentCondition_setCO2(currentCondition_t self, uint16_t value) {
	if(self!=NULL)
	self->co2Data = value;
}

void currentCondition_setHumidity(currentCondition_t self, float value) {
	if (self != NULL)
	self->humidityData= value;
}

void currentCondition_setTemperature(currentCondition_t self, float value) {
	if (self != NULL)
	self->temperatureData = value;
}

void currentCondition_setSound(currentCondition_t self, uint16_t value) {
	if (self != NULL)
	self->soundData = value;
}

//	 DO WE ACTUALLY EVER DO THIS?
void currentCondition_destroy(currentCondition_t self) {
	//if (self == NULL)
	//return;
	//free(self->deviceId);
	//free(self->co2Data);
	//free(self->temperatureData);
	//free(self->humidityData);
	//free(self->soundData);
	//free(self);
}
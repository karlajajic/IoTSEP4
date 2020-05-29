#include <stdlib.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "currentCondition.h"
#include <lora_driver.h>
#include <hal_defs.h>
typedef struct currentCondition currentCondtion;

typedef struct currentCondition {
	uint16_t co2Data;
	int16_t temperatureData;
	uint16_t humidityData;
	uint16_t soundData;
}currentCondition;

currentCondition_t currentCondition_create() {
	currentCondition_t _new_con = calloc(sizeof(currentCondition), 1);
	if (_new_con == NULL)
	return NULL;

	_new_con->temperatureData = 0;
	_new_con->humidityData = 0;
	_new_con->co2Data = 0;

	return _new_con;
}


void currentCondition_setCO2(currentCondition_t self, uint16_t value) {
	if(self!=NULL)
	self->co2Data = value;
}

void currentCondition_setHumidity(currentCondition_t self, uint16_t value) {
	if (self != NULL)
	self->humidityData= value;
}

void currentCondition_setTemperature(currentCondition_t self, int16_t value) {
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
	vPortFree(self);//maybe use this
}
lora_payload_t getcurrentConditionPayload(currentCondition_t self)
{
	lora_payload_t payload;
	payload.port_no = 1;
	payload.len = 4;
	
	payload.bytes[0] = self->temperatureData >> 8;
	payload.bytes[1] = self->temperatureData & 0xFF;

	payload.bytes[2] = self->humidityData >> 8;
	payload.bytes[3] = self->humidityData & 0xFF;
	return payload;
	
}
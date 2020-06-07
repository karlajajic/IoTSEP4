#include <stdlib.h>
#include <stdint.h>
#include "currentCondition.h"
#include <lora_driver.h>


typedef struct currentCondition currentCondtion;

typedef struct currentCondition {
	uint16_t co2Data;
	int16_t temperatureData;
	uint16_t humidityData;
	uint16_t soundData;
}currentCondition;

currentCondition_t currentCondition_create() {
	currentCondition_t _new_con = calloc(1, sizeof(currentCondition));
	if (_new_con == NULL)
	return NULL;

	_new_con->temperatureData = 0;
	_new_con->humidityData = 0;
	_new_con->co2Data = 0;
	_new_con->soundData = 0;

	return _new_con;
}


void currentCondition_setCO2(currentCondition_t self, uint16_t value) {
	if(self!=NULL)
	{
		if(value >= 200 && value <= 10000)
		self->co2Data = value;
	}
}

void currentCondition_setHumidity(currentCondition_t self, uint16_t value) {
	if (self != NULL)
	{
		//the values for humidity is x10
		if (value >= 0 && value <= 1000)
		{
			self->humidityData= value;
		}
	}
	
}

void currentCondition_setTemperature(currentCondition_t self, int16_t value) {
	if (self != NULL)
	{
		//the value for temperature is x10
		if (value >= -400 && value <= 550)
		{
			self->temperatureData = value;
		}
	}
}

void currentCondition_setSound(currentCondition_t self, uint16_t value) {
	if (self != NULL)
	{
		if (value >= 0 && value <= 150)
		{
			self->soundData = value;
		}
	}
	
}

void currentCondition_destroy(currentCondition_t self) {
	if(self == NULL)
		return;
	vPortFree(self);
}

lora_payload_t getcurrentConditionPayload(currentCondition_t self)
{
	lora_payload_t payload;
	payload.port_no = 1;
	payload.len = 8;
	
	int16_t tempData = self->temperatureData;
	
	payload.bytes[0] = tempData >> 8;
	payload.bytes[1] = tempData & 0xFF;

	payload.bytes[2] = self->humidityData >> 8;
	payload.bytes[3] = self->humidityData & 0xFF;
	
	payload.bytes[4] = self->co2Data >> 8;
	payload.bytes[5] = self->co2Data & 0xFF;
	
	payload.bytes[6] = self->soundData >> 8;
	payload.bytes[7] = self->soundData & 0xFF;
	
	return payload;
	
}

lora_payload_t getSimplePayload(currentCondition_t self)
{
	lora_payload_t payload;
	payload.port_no = 1;
	payload.len = 2;
	
	uint16_t one = 0;
	payload.bytes[0] = one >> 8;
	payload.bytes[1] = one & 0xFF;
	
	return payload;
}
#include <ATMEGA_FreeRTOS.h>

#include <stdint.h>
#include <stdlib.h>
#include "device.h"
#include <lora_driver.h>
#include <hal_defs.h>
#include <message_buffer.h>
#include "Configuration.h"
#include "Servo.h"
#include "currentCondition.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <hal_defs.h>
#include <ihal.h>


static EventGroupHandle_t _startMeasureEventGroup;
static EventBits_t _startMeasureBit;

static EventGroupHandle_t _readyMeasuringEventGroup;
static EventBits_t _readyBit;

static MessageBufferHandle_t _uplinkmessageBuffer;

static lora_payload_t _uplink_payload;

typedef struct device device;

typedef struct device { //add all drivers
	co2reader_t co2reader;
	humAndTempReader_t humAndTempReader;
	soundReader_t soundReader;
	currentCondition_t currentCondition;
	TaskHandle_t handleTask;
}device;

//devided so we can test
void device_executeTask(void* self) {
	for (;;)
	{
		device_startMeasuring((device_t)self);
	}
}

device_t device_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
EventGroupHandle_t readyEventGroup, EventBits_t readyBit, co2reader_t co2Reader, humAndTempReader_t humAndTempReader,soundReader_t soundReader, MessageBufferHandle_t uplinkMessageBuffer){

	device_t _new_device = calloc(1, sizeof(device));
	if (_new_device == NULL)
		return NULL;

	_new_device->co2reader = co2Reader;
	_new_device->humAndTempReader = humAndTempReader;
	_new_device->soundReader=soundReader;
	currentCondition_t currentCond = currentCondition_create();
	_new_device->currentCondition = currentCond;

	_startMeasureEventGroup = startMeasureEventGroup;
	_startMeasureBit = startMeasureBit;

	_readyMeasuringEventGroup = readyEventGroup;
	_readyBit = readyBit;
	
	_uplinkmessageBuffer=uplinkMessageBuffer;

	xTaskCreate(
		device_executeTask,
		"Device",
		stack + 200,
		_new_device,
		priority,
		&_new_device->handleTask
	);

	return _new_device;
}

void device_startMeasuring(device_t self) {
	//we should first check if device is on, get that from lora and add new eventBit 
	
	bool* works = pvPortMalloc(sizeof(bool));
	configuration_getWorking(works);
	if (*works == true)
	{

		bool* ventilate = pvPortMalloc(sizeof(bool));
		configuration_getVentilation(ventilate);
		if(*ventilate == true)
		{
			servo_open();
		} 
		else
		{
			servo_close();
		}

	////tell sensors to start meassuring 
	xEventGroupSetBits(_startMeasureEventGroup, _startMeasureBit);

	//wait for sensors to read data
	EventBits_t uxBits = xEventGroupWaitBits(_readyMeasuringEventGroup, 
		_readyBit, 
		pdTRUE, 
		pdTRUE,
		portMAX_DELAY);
	
	if ((uxBits & (_readyBit)) == (_readyBit)) {
		currentCondition_setCO2(self->currentCondition,co2Reader_getCO2(self->co2reader));
		
		currentCondition_setSound(self->currentCondition,soundReader_getSound(self->soundReader));
		
		currentCondition_setTemperature(self->currentCondition,humAndTempReader_getTemperature(self->humAndTempReader));
		
		int16_t humidity = humAndTempReader_getHumidity(self->humAndTempReader);
		currentCondition_setHumidity(self->currentCondition, humidity);
		
		
		printf("CO2 is: %u\n", co2Reader_getCO2(self->co2reader));
		printf("Temperature is: %d\n", humAndTempReader_getTemperature(self->humAndTempReader));
		printf("Humidity is: %u\n", humAndTempReader_getHumidity(self->humAndTempReader));
		printf("Sound is: %u\n", soundReader_getSound(self->soundReader));
		
		/*Perhaps loraPayload is not a good idea to be here*/
		_uplink_payload = getcurrentConditionPayload(self->currentCondition);
		
		xMessageBufferSend(_uplinkmessageBuffer,(void*) &_uplink_payload,sizeof(_uplink_payload),portMAX_DELAY);
		vTaskDelay(30000);
		
	}
	//if the device is not on, wait a bit and check if anything is changed
	}
	else
	{
		_uplink_payload = getSimplePayload(self->currentCondition);
		xMessageBufferSend(_uplinkmessageBuffer,(void*) &_uplink_payload,sizeof(_uplink_payload),portMAX_DELAY);
		vTaskDelay(30000);
	} 
}

#include "device.h"

extern int deviceId;

static EventGroupHandle_t _startMeasureEventGroup;
static EventBits_t _startMeasureBit;

static EventGroupHandle_t _readyEventGroup;
static EventBits_t _readyBit;

static MessageBufferHandle_t _uplinkmessageBuffer;

typedef struct device device;

typedef struct device { //add all drivers
	co2reader_t co2reader;
	humAndTempReader_t humAndTempReader;
	currentCondition_t currentCondition;
	TaskHandle_t handleTask;
}device;

device_t device_create(UBaseType_t priority, UBaseType_t stack, EventGroupHandle_t startMeasureEventGroup, EventBits_t startMeasureBit,
	EventGroupHandle_t readyEventGroup, EventBits_t readyBit, co2reader_t co2Reader, humAndTempReader_t humAndTempReader, MessageBufferHandle_t uplinkMessageBuffer){

	device_t _new_device = calloc(sizeof(device), 1);
	if (_new_device == NULL)
		return NULL;

	_new_device->co2reader = co2Reader;
	_new_device->humAndTempReader = humAndTempReader;
	_new_device->currentCondition = currentCondition_create(deviceId);

	_startMeasureEventGroup = startMeasureEventGroup;
	_startMeasureBit = startMeasureBit;

	_readyEventGroup = readyEventGroup;
	_readyBit = readyBit;
	
	_uplinkmessageBuffer=uplinkMessageBuffer;

	xTaskCreate(
		device_executeTask,
		"Device",
		stack,
		_new_device,
		priority,
		&_new_device->handleTask
	);

	printf("device up");

	return _new_device;
}

//devided so we can test
void device_executeTask(device_t self) {
	for (;;)
		device_startMeasuring(self);
}

void device_startMeasuring(device_t self) {
	//we should first check if device is on, get that from lora and add new eventBit 

	//tell sensors to start meassuring 
	xEventGroupSetBits(_startMeasureEventGroup, _startMeasureBit);
	printf("device has set bits");

	//wait for sensors to read data
	EventBits_t uxBits = xEventGroupWaitBits(_readyEventGroup, //eventGroup it is interested in
		_readyBit, //bits it is interested in
		pdTRUE, //clears the bits 
		pdTRUE, //waits for both bits to be set
		portMAX_DELAY); //waits forever if needed

	if ((uxBits & (_readyBit)) == (_readyBit)) {
		printf("device got done bit\n");
		printf("CO2 is: %u\n", co2Reader_getCO2(self->co2reader));
		printf("Temperature is: %d\n", device_getTemperatureData(self));
		printf("Humidity is: %d\n", device_getHumidityData(self));
		
		device_setCO2ToCurrent(self, device_getCO2Data(self));
		device_setTemperatureToCurrent(self, device_getTemperatureData(self));
		device_setHumidityToCurrent(self, device_getHumidityData(self));
		
		
		/*Perhaps loraPayload is not a good idea to be here*/
		lora_payload_t payload = getcurrentConditionPayload(self->currentCondition);
		
		size_t bytesToSend = xMessageBufferSend(_uplinkmessageBuffer,(void*) &payload,sizeof(payload),portMAX_DELAY);
		
		
	}
}

//	 DO WE ACTUALLY EVER DO THIS?
void device_destroy(device_t self) { //destroy all drivers 
	/*if (self == NULL)
		return;

	vTaskDelete(self->handleTask);
	free(self->handleTask);
	free(self->co2reader);
	free(self);*/
}

currentCondition_t device_getCurrentCondition(device_t self) {
	return self->currentCondition;
}

void device_setHumidityToCurrent(device_t self, uint16_t value)
{
	currentCondition_setHumidity(self->currentCondition,value);
}


uint16_t device_getHumidityData(device_t self)
{
	if (self->humAndTempReader!=NULL)
	{
		return humAndTempReader_getHumidity(self->humAndTempReader);
	}
	else
	return -1;
}

void device_setCO2ToCurrent(device_t self, uint16_t value) {
	currentCondition_setCO2(self->currentCondition, value);
}

uint16_t device_getCO2Data(device_t self) {
	if (self->co2reader != NULL)
		return co2Reader_getCO2(self->co2reader);
	else return -1;
}

void device_setTemperatureToCurrent(device_t self, int16_t value)
{
	
	currentCondition_setTemperature(self->currentCondition,value);
	
}


int16_t device_getTemperatureData(device_t self)
{
	if (self->humAndTempReader!=NULL)
	{
		return humAndTempReader_getTemperature(self->humAndTempReader);
	}
	else
	return -1;
	
}
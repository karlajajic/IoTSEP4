#include "co2Reader.h"
#include "device.h"

extern int deviceId;

typedef struct device device;

typedef struct device { //add all drivers
	co2reader_t co2reader;

	currentCondition_t currentCondition;
	TaskHandle_t handleTask;
}device;

device_t device_create(void) {
	device_t _new_device = calloc(sizeof(device), 1);
	if (_new_device == NULL)
	return NULL;

	_new_device->co2reader = co2Reader_create();
	_new_device->currentCondition = currentCondition_create(deviceId);

	xTaskCreate(
	device_executeTask,
	"Device",
	DEVICE_TASK_STACK,
	_new_device,
	TASK_DEVICE_PRIORITY,
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
	xEventGroupSetBits(startMeasureEventGroup, BIT_MEASURE_CO2);//add the rest of the bits when implemented
	
	printf("device has set bits");

	//wait for sensors to read data
	EventBits_t uxBits = xEventGroupWaitBits(doneMeasuringEventGroup, //eventGroup it is interested in
	BIT_DONE_MEASURE_CO2, //bits it is interested in, ADD THE REST WHEN IMPLEMENTED
	pdTRUE, //clears the bits
	pdTRUE, //waits for both bits to be set
	portMAX_DELAY); //waits forever if needed

	//if ((uxBits & (BIT_TEMPERATURE | BIT_HUMIDITY)) == (BIT_TEMPERATURE | BIT_HUMIDITY))
	if ((uxBits & (BIT_DONE_MEASURE_CO2)) == (BIT_DONE_MEASURE_CO2)) {
		printf("device got done bit");

		device_setCO2ToCurrent(self, device_getCO2Data(self));
		//add the rest
	}
}

//	 DO WE ACTUALLY EVER DO THIS?
void device_destroy(device_t self) { //destroy all drivers
	if (self == NULL)
	return;

	vTaskDelete(self->handleTask);
	free(self->handleTask);
	free(self->co2reader);
	free(self);
}

currentCondition_t device_getCurrentCondition(device_t self) {
	return self->currentCondition;
}

void device_setCO2ToCurrent(device_t self, uint16_t value) {
	currentCondition_setCO2(self->currentCondition, value);
}

uint16_t device_getCO2Data(device_t self) {
	if (self->co2reader != NULL)
	return co2Reader_getCO2(self->co2reader);
	else return -1;
}
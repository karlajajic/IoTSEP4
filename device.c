/*
 * device.c
 *
 * Created: 1.5.2020. 13:56:28
 *  Author: PC HP
 */ 

 #include <stdlib.h>
 #include "co2Reader.h"
 #include "device.h"

 extern int deviceId;

 typedef struct device device;

 typedef struct device { //add all drivers
	 co2reader_t co2reader;
 }device;

 device_t device_create(void) {
	 device_t _new_device = calloc(sizeof(device), 1);
	 if (_new_device == NULL)
	 return NULL;

	 _new_device->co2reader = co2Reader_create();
	 return _new_device;
 }

 void device_destroy(device_t self) { //destroy all drivers
	 if (self == NULL)
	 return;
	 free(self->co2reader);
	 free(self);
 }

 currentCondition_t device_getCurrentCondition(uint16_t co2Data, uint16_t temperatureData, uint16_t humidityData, uint16_t soundData) {
	 return currentCondition_create(deviceId, co2Data, temperatureData, humidityData, soundData);
 }

 uint16_t device_getCO2Data(device_t self) {
	 if (self->co2reader != NULL)
	 return co2Reader_getCO2(self->co2reader);
	 else return -1;
 }
/*
 * device.h
 *
 * Created: 1.5.2020. 13:53:22
 *  Author: PC HP
 */ 

 #pragma once
 #include "currentCondition.h"
 #include <stdint.h>

 typedef struct device* device_t;

 device_t device_create(void);
 void device_destroy(device_t self);
 currentCondition_t device_getCurrentCondition( uint16_t co2Data, uint16_t temperatureData, uint16_t humidityData, uint16_t soundData);

 uint16_t device_getCO2Data(device_t self);


 //handleCommand??????

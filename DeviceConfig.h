#pragma once

//include all of the needed libraries
#include <stdint.h>
#include <stdlib.h>


//#include "FreeRTOS.h"
#include "ATMEGA_FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
#include "semphr.h"
#include "message_buffer.h"

//define priority individually for each task
#define TASK_HUMIDITY_SENSOR_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define TASK_CO2_SENSOR_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define TASK_DEVICE_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define TASK_LORA_DRIVER_PRIORITY			( tskIDLE_PRIORITY + 1 )

//define task stack for each task
#define HUMIDITY_TASK_STACK			(configMINIMAL_STACK_SIZE)
#define CO2_TASK_STACK				(configMINIMAL_STACK_SIZE)
#define DEVICE_TASK_STACK			(configMINIMAL_STACK_SIZE)
#define LORA_DRIVER_TASK_STACK		(configMINIMAL_STACK_SIZE)

//defining bits that will be the 'flags' for event group
#define BIT_MEASURE_HUMIDITY (1 << 0)
#define BIT_MEASURE_CO2		 (1 << 1)

#define BIT_DONE_MEASURE_HUMIDITY (1 << 4)
#define BIT_DONE_MEASURE_CO2	  (1 << 5)

//used for appController to prompt sensors to start with readings
EventGroupHandle_t startMeasureEventGroup;
//used for appController to check if sensors are done with readings
EventGroupHandle_t doneMeasuringEventGroup;

//used for communication between appController task and LoraDriver task
MessageBufferHandle_t sendCurrentDataMessageBuffer;


////used to control printouts to console
//xSemaphoreHandle printerMutex;
//
////used for appController to check if sensors are done with readings
//EventGroupHandle_t eventGroup;
//
////used for communication between appController taks and LoraDriver task
//MessageBufferHandle_t messageBuffer;

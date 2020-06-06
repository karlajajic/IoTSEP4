/*
 * SEP4IOT.c
 *
 * Created: 1.5.2020. 11:53:47
 * Author : PC HP
 */ 

#include <stdio.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>

#include <hal_defs.h>
#include <ihal.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include <FreeRTOSTraceDriver.h>
#include <stdio_driver.h>
#include <serial.h>
#include <message_buffer.h>

#include "task.h"
#include "event_groups.h"
#include "semphr.h"

#include "device.h"
#include "currentCondition.h"
#include "co2Reader.h"
#include "humidityAndTemperature.h"
#include "UpLinkHandler.h"
#include "DownLinkHandler.h"
#include "Configuration.h"
#include "Servo.h"
#include "SoundReader.h"



// Needed for LoRaWAN
#include <lora_driver.h>

//define priority individualy for each task
#define TASK_SOUND_SENSOR_PRIORITY		( tskIDLE_PRIORITY + 2)
#define TASK_HUMIDITY_SENSOR_PRIORITY	( tskIDLE_PRIORITY + 2 )
#define TASK_CO2_SENSOR_PRIORITY		( tskIDLE_PRIORITY + 2)
#define TASK_DEVICE_PRIORITY			( tskIDLE_PRIORITY + 4 )
#define TASK_LORA_DRIVER_PRIORITY		( tskIDLE_PRIORITY + 1 )
#define TASK_LORA_DRIVER_PRIORITYDOWN	( tskIDLE_PRIORITY + 1 )

//define task stack for each task
#define SOUND_TASK_STACK				(configMINIMAL_STACK_SIZE )
#define HUMIDITY_TASK_STACK				(configMINIMAL_STACK_SIZE )
#define CO2_TASK_STACK					(configMINIMAL_STACK_SIZE )
#define DEVICE_TASK_STACK				(configMINIMAL_STACK_SIZE )
#define LORA_DRIVER_TASK_STACK			(configMINIMAL_STACK_SIZE )

//defining bits that will be the 'flags' for event group
			
#define BIT_MEASURE_HUMIDITY			(1 << 0)
#define BIT_MEASURE_CO2					(1 << 3)
#define BIT_MEASURE_SOUND				(1 << 2)
#define ALL_BIT_MEASURE					(BIT_MEASURE_CO2 | BIT_MEASURE_HUMIDITY | BIT_MEASURE_SOUND)

#define DEVICE_SET_BIT					(1 << 1)

#define BIT_DONE_MEASURE_HUMIDITY		(1 << 4)
#define BIT_DONE_MEASURE_CO2			(1 << 5)
#define BIT_DONE_MEASURE_SOUND			(1 << 6)
#define ALL_BIT_DONE_MEASURE			(BIT_DONE_MEASURE_CO2 | BIT_DONE_MEASURE_HUMIDITY | BIT_DONE_MEASURE_SOUND)


//used for appController to prompt sensors to start with readings
EventGroupHandle_t startMeasureEventGroup;
//used for appController to check if sensors are done with readings
EventGroupHandle_t readyEventGroup;
//used for
EventGroupHandle_t waitEventGroup;

// define semaphore handle

MessageBufferHandle_t xMessageBuffer;
MessageBufferHandle_t _downlinkMessagebuffer;
lora_payload_t payload;


// Prototype for LoRaWAN handler
void lora_handler_create(UBaseType_t lora_handler_task_priority);

/*-----------------------------------------------------------*/
void create_tasks_and_semaphores(void)
{
	//// Semaphores are useful to stop a Task proceeding, where it should be paused to wait,
	//// because it is sharing a resource, such as the Serial port.
	//// Semaphores should only be used whilst the scheduler is running, but we can set it up here.
	//if ( xTestSemaphore == NULL )  // Check to confirm that the Semaphore has not already been created.
	//{
		//xTestSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore.
		//if ( ( xTestSemaphore ) != NULL )
		//{
			//xSemaphoreGive( ( xTestSemaphore ) );  // Make the mutex available for use, by initially "Giving" the Semaphore.
		//}
	//}
	
	//startMeasureEventGroup = xEventGroupCreate();
	//readyEventGroup = xEventGroupCreate();
//
	//xMessageBuffer = xMessageBufferCreate(100);
	//_semaphore = xSemaphoreCreateMutex();
	
	//_downlinkMessagebuffer = xMessageBufferCreate(sizeof(lora_payload_t)*2);
	
	configuration_create();
	
	humAndTempReader_t humidityAndTemperature = humAndTempReader_create(TASK_HUMIDITY_SENSOR_PRIORITY, HUMIDITY_TASK_STACK, 
	startMeasureEventGroup, BIT_MEASURE_HUMIDITY, readyEventGroup, BIT_DONE_MEASURE_HUMIDITY);
	
	co2reader_t co2reader = co2Reader_create(TASK_CO2_SENSOR_PRIORITY, CO2_TASK_STACK, startMeasureEventGroup, BIT_MEASURE_CO2,
	readyEventGroup, BIT_DONE_MEASURE_CO2);
	
	//co2reader_t co2reader = NULL;
	
	
	
	soundReader_t soundReader = soundReader_create(TASK_SOUND_SENSOR_PRIORITY, SOUND_TASK_STACK, startMeasureEventGroup, BIT_MEASURE_SOUND,
	readyEventGroup, BIT_DONE_MEASURE_SOUND);	
	//soundReader_t soundReader = NULL;
	
	servo_initialise();
	
	device_create(TASK_DEVICE_PRIORITY, DEVICE_TASK_STACK, startMeasureEventGroup, ALL_BIT_MEASURE,
	readyEventGroup, ALL_BIT_DONE_MEASURE, co2reader, humidityAndTemperature,soundReader, xMessageBuffer, waitEventGroup, DEVICE_SET_BIT);
	
	lora_UpLinkHandler_create(TASK_LORA_DRIVER_PRIORITY,xMessageBuffer, waitEventGroup, DEVICE_SET_BIT);
	lora_DownLinkHandler_create(TASK_LORA_DRIVER_PRIORITYDOWN,_downlinkMessagebuffer);
}

/*-----------------------------------------------------------*/
void initialiseSystem()
{
	startMeasureEventGroup = xEventGroupCreate();
	readyEventGroup = xEventGroupCreate();
	waitEventGroup = xEventGroupCreate();

	xMessageBuffer = xMessageBufferCreate(100);

	_downlinkMessagebuffer = xMessageBufferCreate(sizeof(lora_payload_t)*2);
	// Set output ports for leds used in the example
	DDRA |= _BV(DDA0) | _BV(DDA7);
	//// Initialise the trace-driver to be used together with the R2R-Network
	//trace_init();
	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	stdioCreate(ser_USART0);
	
	
	
	// vvvvvvvvvvvvvvvvv BELOW IS LoRaWAN initialisation vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
	// Initialise the HAL layer and use 5 for LED driver priority
	hal_create(5);
	// Initialise the LoRaWAN driver with a down-link buffer
	
	
	lora_driver_create(LORA_USART, _downlinkMessagebuffer);
	// Create LoRaWAN task and start it up with priority 3 
	
	hih8120Create();
	
	
	// Let's create some tasks
	
}
/*-----------------------------------------------------------*/
int main(void)
{
	initialiseSystem(); // Must be done as the very first thing!!
	create_tasks_and_semaphores();
	printf("Program Started!!\n");
	vTaskStartScheduler(); // Initialise and run the freeRTOS scheduler. Execution should never return from here.
	
	/* Replace with your application code */
	while (1)
	{
	}
}


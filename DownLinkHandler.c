
/*
 * DownLinkHandler.c
 *
 * Created: 12/05/2020 21:44:49
 *  Author: Haralambi
 */ 
#include <stddef.h>
#include <stdio.h>
#include "DownLinkHandler.h"
#include "Configuration.h"
#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
/*Leds*/
#include <iled.h>
#include <message_buffer.h>



#define LORA_appEUI "  fbf6ad621cf57cd7"
#define LORA_appKEY "bf9a206660a448b3892f0bd64935e4d5"

static lora_payload_t _downlink_payload;
//static bool _isSet; 
//int16_t temperature_setting; // Temperature

void lora_DownLinkHandler_startTask(void* xMessageBuffer){
	for(;;)
	{
		lora_DownLinkHandler_task((MessageBufferHandle_t)xMessageBuffer);
		
	}
}

void lora_DownLinkHandler_create(UBaseType_t lora_handler_task_priority, MessageBufferHandle_t xMessageBuffer)
{
	
	//_isSet=isSet;
	xTaskCreate(
	lora_DownLinkHandler_startTask
	,  (const portCHAR *)"LRDHHand"  // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  xMessageBuffer
	,  lora_handler_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	
}


	void lora_DownLinkHandler_task(MessageBufferHandle_t xMessageBuffer)
	{
				
		//size_t xBytesReceived;
		//xBytesReceived = 
		xMessageBufferReceive(xMessageBuffer,(void*) &_downlink_payload,sizeof(lora_payload_t),portMAX_DELAY);
		printf("PayLoad bytes are: %d, %d\n Payload lenght is %d\n", _downlink_payload.bytes[0] ,_downlink_payload.bytes[1],_downlink_payload.len);
		
		//Two bytes eg. 44/33
		if(_downlink_payload.len==2)
		{
			uint8_t command = _downlink_payload.bytes[0] + _downlink_payload.bytes[1];
			bool* value = pvPortMalloc(sizeof(bool));
			
		
			switch(command)
			{
			//D0
			case 74:
				configuration_setWorking(false);
				configuration_getWorking(value);
				printf("The bool for device is set to %d",*value);
				break;
			//D1
			case 75 :
				configuration_setWorking(true);
				configuration_getWorking(value);
				printf("The bool for device is set to %d",*value);
				break;
			//V0
			case 86:
				configuration_setVentilation(false);
				configuration_getVentilation(value);
				printf("The bool for ventilation is set to %d",*value);
			break;
			//V1
			case 87:
				configuration_setVentilation(true);
				configuration_getVentilation(value);
				printf("The bool for ventilation is set to %d",*value);
				break;
			default:
				printf("Invalid command");
				break;
		}
	}
						
}








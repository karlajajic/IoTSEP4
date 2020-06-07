
#include <ATMEGA_FreeRTOS.h>
#include <stddef.h>
#include <stdio.h>
#include "DownLinkHandler.h"
#include "Configuration.h"
#include <stdlib.h>

#include <lora_driver.h>
/*Leds*/
#include <iled.h>
#include <message_buffer.h>


static lora_payload_t _downlink_payload;

void lora_DownLinkHandler_startTask(void* xMessageBuffer){
	for(;;)
	{
		lora_DownLinkHandler_task((MessageBufferHandle_t)xMessageBuffer);
		
	}
}

void lora_DownLinkHandler_create(UBaseType_t lora_handler_task_priority, MessageBufferHandle_t xMessageBuffer)
{
	
	
	xTaskCreate(
	lora_DownLinkHandler_startTask
	,  (const portCHAR *)"LRDHHand"  
	,  configMINIMAL_STACK_SIZE+200  
	,  xMessageBuffer
	,  lora_handler_task_priority  
	,  NULL );
	
}


	void lora_DownLinkHandler_task(MessageBufferHandle_t xMessageBuffer)
	{
				
		xMessageBufferReceive(xMessageBuffer,(void*) &_downlink_payload,sizeof(lora_payload_t),portMAX_DELAY);
		printf("PayLoad bytes are: %d, %d\n Payload lenght is %d\n", _downlink_payload.bytes[0] ,_downlink_payload.bytes[1],_downlink_payload.len);
		

		if(_downlink_payload.len==2)
		{
			uint8_t command = _downlink_payload.bytes[0] + _downlink_payload.bytes[1];
			bool* value = pvPortMalloc(sizeof(bool));
			
			switch(command)
			{
			//D0
			case 0x74:
				configuration_setWorking(false);
				configuration_getWorking(value);
				printf("The bool for device is set to %d\n",*value);
				break;
			//D1
			case 0x75 :
				configuration_setWorking(true);
				configuration_getWorking(value);
				printf("The bool for device is set to %d\n",*value);
				break;
			//V0
			case 0x86:
				configuration_setVentilation(false);
				configuration_getVentilation(value);
				printf("The bool for ventilation is set to %d\n",*value);
			break;
			//V1
			case 0x87:
				configuration_setVentilation(true);
				configuration_getVentilation(value);
				printf("The bool for ventilation is set to %d\n",*value);
				break;
			default:
				printf("Invalid command\n");
				break;
		}
	}
						
}








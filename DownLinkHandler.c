
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
		vTaskDelay(3000);
	}
}

void lora_DownLinkHandler_create(MessageBufferHandle_t xMessageBuffer)
{
	
	//_isSet=isSet;
	
}

//static void _lora_setup(void)
//{
	//e_LoRa_return_code_t rc;
	//led_slow_blink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa
//
	////The below code is required to be executed only once due to the fact that these MAC settings
	//// can be stored inside of the tranciever
	////START ON ONE TIME USE CODE
	//
	//// Factory reset the transceiver
	//printf("FactoryReset >%s<\n", lora_driver_map_return_code_to_text(lora_driver_rn2483_factory_reset()));
	//
	//// Configure to EU868 LoRaWAN standards
	//printf("Configure to EU868 >%s<\n", lora_driver_map_return_code_to_text(lora_driver_configure_to_eu868()));
//
	//// Get the transceivers HW EUI
	//rc = lora_driver_get_rn2483_hweui(_out_buf);
	//printf("Get HWEUI >%s<: %s\n",lora_driver_map_return_code_to_text(rc), _out_buf);
//
	//// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	//printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_map_return_code_to_text(lora_driver_set_device_identifier(_out_buf)));
//
	//// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	//printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_map_return_code_to_text(lora_driver_set_otaa_identity(LORA_appEUI,LORA_appKEY,_out_buf)));
//
	//// Save all the MAC settings in the transceiver
	//printf("Save mac >%s<\n",lora_driver_map_return_code_to_text(lora_driver_save_mac()));
//
//
	////END OF ONE TIME USE CODE
//
//
	//// Enable Adaptive Data Rate
	//printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_map_return_code_to_text(lora_driver_set_adaptive_data_rate(LoRa_ON)));
//
	//// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	//printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_map_return_code_to_text(lora_driver_set_receive_delay(500)));
//
	//// Join the LoRaWAN
	//uint8_t maxJoinTriesLeft = 10;
	//
	//do {
		//rc = lora_driver_join(LoRa_OTAA);
		//printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_map_return_code_to_text(rc));
//
		//if ( rc != LoRa_ACCEPTED)
		//{
			//// Make the red led pulse to tell something went wrong
			//led_long_puls(led_ST1); // OPTIONAL
			//// Wait 5 sec and lets try again
			//vTaskDelay(pdMS_TO_TICKS(5000UL));
		//}
		//else
		//{
			//break;
		//}
	//} while (--maxJoinTriesLeft);
//
	//if (rc == LoRa_ACCEPTED)
	//{
		//// Connected to LoRaWAN :-)
		//// Make the green led steady
		//led_led_on(led_ST2); // OPTIONAL
	//}
	//else
	//{
		//// Something went wrong
		//// Turn off the green led
		//led_led_off(led_ST2); // OPTIONAL
		//// Make the red led blink fast to tell something went wrong
		//led_fast_blink(led_ST1); // OPTIONAL
//
		//// Switches to another task with a priority of equal or greater size. If no such task is found it stays on the same.
		////Perhaps it should be in a loop if the task itself is in  a loop
		//while (1)
		//{
			//taskYIELD();
			//
		//}
	//}
//}

	void lora_DownLinkHandler_task(MessageBufferHandle_t xMessageBuffer)
	{
		
		//size_t xBytesToSend;
		

				//// hardware reset of lorawan transceiver
				//lora_driver_reset_rn2483(1);
				//vTaskDelay(2);
				//lora_driver_reset_rn2483(0);
				//// give it a chance to wakeup
				//vTaskDelay(150);
//
				//lora_driver_flush_buffers(); // get rid of first version string from module after reset!
//
				//_lora_setup();
				
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








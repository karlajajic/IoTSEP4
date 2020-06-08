#include <ATMEGA_FreeRTOS.h>

#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "UpLinkHandler.h"

#include <lora_driver.h>
#include <iled.h>

#define LORA_appEUI "fbf6ad621cf57cd7"
#define LORA_appKEY "bf9a206660a448b3892f0bd64935e4d5"
#include <message_buffer.h>

static char _out_buf[100];
/*Leds*/
#include <iled.h>

static lora_payload_t _uplink_payload;
static MessageBufferHandle_t _buffer;
static bool isSet=false;

void lora_UpLinkHandler_startTask(void* messageBuffer){
	for(;;)
	{
		lora_UpLinkHandler_task((MessageBufferHandle_t)messageBuffer);
	}
}


void lora_UpLinkHandler_create(UBaseType_t priority, UBaseType_t stack, MessageBufferHandle_t messageBuffer)
{
	_buffer = messageBuffer;
	
	xTaskCreate(
	lora_UpLinkHandler_startTask
	,  (const portCHAR *)"LRUpHand"
	,  stack+200
	,  (void*)messageBuffer
	,  priority
	,  NULL );
}


static void _lora_setup(void)
{
	e_LoRa_return_code_t rc;
	led_slow_blink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa


	//The below code is required to be executed only once due to the fact that these MAC settings
	//can be stored inside of the transceiver.
	//That being said it runs every time when a device is being started.
	
	//START OF ONE TIME USE CODE

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_map_return_code_to_text(lora_driver_rn2483_factory_reset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_map_return_code_to_text(lora_driver_configure_to_eu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_get_rn2483_hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_map_return_code_to_text(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_map_return_code_to_text(lora_driver_set_device_identifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_map_return_code_to_text(lora_driver_set_otaa_identity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_map_return_code_to_text(lora_driver_save_mac()));

	//END OF ONE TIME USE CODE
	
	
	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_map_return_code_to_text(lora_driver_set_adaptive_data_rate(LoRa_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_map_return_code_to_text(lora_driver_set_receive_delay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;
	
	do {
		rc = lora_driver_join(LoRa_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_map_return_code_to_text(rc));

		if ( rc != LoRa_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			led_long_puls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LoRa_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		led_led_on(led_ST2); // OPTIONAL
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		led_led_off(led_ST2);
		// Make the red led blink fast to tell something went wrong
		led_fast_blink(led_ST1);

		while (1)
		{
			taskYIELD();
		}
	}
}

void lora_UpLinkHandler_task(MessageBufferHandle_t messageBuffer)
{
	size_t xBytesToSend;
	char rxData[50];
	if (!isSet)
	{
		// Hardware reset of LoRaWAN transceiver
		lora_driver_reset_rn2483(1);
		vTaskDelay(2);
		lora_driver_reset_rn2483(0);
		vTaskDelay(150);

		lora_driver_flush_buffers(); // get rid of first version string from module after reset!

		_lora_setup();
		isSet=true;
	}
	
	xBytesToSend = xMessageBufferReceive(messageBuffer, (void*) &_uplink_payload,
	sizeof(rxData),0);
	if(xBytesToSend >= sizeof(uint8_t)*2)
	{
		/**< Status led ST4 (BLUE)*/
		//Makes the led light up in a short period.
		led_short_puls(led_ST4);
		
		printf("Upload Message >%s<\n", lora_driver_map_return_code_to_text(lora_driver_sent_upload_message(false, &_uplink_payload)));
	}
}









/*
 * DownLinkHandler.h
 *
 * Created: 16/05/2020 21:59:25
 *  Author: Haralambi
 */ 
#pragma once

#include "ATMEGA_FreeRTOS.h"
#include "portmacro.h"
#include "message_buffer.h"

void lora_DownLinkHandler_create(MessageBufferHandle_t xMessageBuffer);
void lora_DownLinkHandler_task(MessageBufferHandle_t xMessageBuffer);
void lora_DownLinkHandler_startTask(void* xMessageBuffer);
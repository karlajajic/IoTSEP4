
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

void lora_DownLinkHandler_create(UBaseType_t lora_handler_task_priority, MessageBufferHandle_t xMessageBuffer,bool isSet);
void lora_DownLinkHandler_task(MessageBufferHandle_t xMessageBuffer);
lora_DownLinkHandler_startTask(MessageBufferHandle_t xMessageBuffer);
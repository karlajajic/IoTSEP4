#pragma once

#include "ATMEGA_FreeRTOS.h"
#include "portmacro.h"
#include "message_buffer.h"

void lora_UpLinkHandler_create(UBaseType_t priority, UBaseType_t stack, MessageBufferHandle_t messageBuffer);
void lora_UpLinkHandler_task(MessageBufferHandle_t messageBuffer);
void lora_UpLinkHandler_startTask(void* messageBuffer);
#pragma once

#include "ATMEGA_FreeRTOS.h"
#include "portmacro.h"
#include "message_buffer.h"

void lora_DownLinkHandler_create(UBaseType_t priority, UBaseType_t stack, MessageBufferHandle_t messageBuffer);
void lora_DownLinkHandler_task(MessageBufferHandle_t messageBuffer);
void lora_DownLinkHandler_startTask(void* messageBuffer);
#pragma once

#include "ATMEGA_FreeRTOS.h"
#include "portmacro.h"
#include "message_buffer.h"

void lora_UpLinkHandler_create(MessageBufferHandle_t xMessageBuffer);
void lora_UpLinkHandler_task(MessageBufferHandle_t xMessageBuffer);
void lora_UpLinkHandler_startTask(void* xMessageBuffer);
#include "DeviceConfig.h"
#include "device.h"

static device_t _device;
int deviceId = 100;

// --------------------------------------------------------------------------------------

static void _runApplication(void) {
	startMeasureEventGroup = xEventGroupCreate();
	doneMeasuringEventGroup = xEventGroupCreate();
	
	
	lora_UpLinkHandler_create();
	device_create();

	vTaskStartScheduler();
}

// --------------------------------------------------------------------------------------
void main_blinky(void)
{
	_runApplication();
}
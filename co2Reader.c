#include"co2Reader.h"

typedef struct co2reader co2reader;

typedef struct co2reader {
	uint8_t value;
	TaskHandle_t handleTask;
}co2reader;


co2reader_t co2Reader_create(void) {
	co2reader_t _new_reader = calloc(sizeof(co2reader), 1);
	if (_new_reader == NULL)
	return NULL;

	_new_reader->value = 0;

	xTaskCreate(
	co2Reader_executeTask,
	"CO2Reader",
	CO2_TASK_STACK,
	_new_reader,
	TASK_CO2_SENSOR_PRIORITY,
	&_new_reader->handleTask
	);

	printf("co2 up");

	return _new_reader;
}
void co2Reader_destroy(co2reader_t self) {
	if (self == NULL)
	return;

	//delete will clear the allocated memory to the task + we need to remove everything else
	vTaskDelete(self->handleTask);

	//free the values from struct (without pointer, later the pointer
	free(self->handleTask);
	free(self->value);
	free(self);
}

//actual task, methods devided so that it is possible to test
void co2Reader_executeTask(co2reader_t self) {
	for (;;) {
		co2Reader_measure(self);
	}
}

void co2Reader_measure(co2reader_t self) {//dummy
	EventBits_t uxBits = xEventGroupWaitBits(startMeasureEventGroup, //eventGroup
	BIT_MEASURE_CO2, //bits it is interested in
	pdTRUE, //clears the bits
	pdTRUE, //waits for both bits to be set
	portMAX_DELAY); //wait

	if ((uxBits & (BIT_MEASURE_CO2)) == (BIT_MEASURE_CO2)) {
		uint8_t no = self->value;
		no++;
		self->value = no;
		printf("co2 done bit set");

		vTaskDelay(2500); //pretend it takes some time

		//set done bit so that device knows meassurement is done
		xEventGroupSetBits(doneMeasuringEventGroup, BIT_DONE_MEASURE_CO2);
	}
}

uint8_t co2Reader_getCO2(co2reader_t self) {
	return self->value;
}
#include"humidityAndTemperature.h"

typedef struct humidityAndTemperature humidityAndTemperature;

typedef struct humidityAndTemperature {
	float humidity;
	float temperature;
	TaskHandle_t handleTask;
}humidityAndTemperature;


humAndTempReader_t humAndTempReader_create(void) {
	humAndTempReader_t _new_reader = calloc(sizeof(humidityAndTemperature), 1);
	if (_new_reader == NULL)
	return NULL;

	_new_reader->humidity = 0.0;
	_new_reader->temperature = 0.0;

	hih8120Create();

		xTaskCreate(
		humAndTempReader_executeTask,
		"HumAndTempReader",
		HUMIDITY_TASK_STACK,
		_new_reader,
		TASK_HUMIDITY_SENSOR_PRIORITY,
		&_new_reader->handleTask
		);
		printf("humidity and temperature ready");
	
	return _new_reader;
}
void humAndTempReader_destroy(humAndTempReader_t self) {
	if (self == NULL)
	return;

	//delete will clear the allocated memory to the task + we need to remove everything else
	vTaskDelete(self->handleTask);

	//free the values from struct (without pointer, later the pointer
	//free(self->handleTask);
	//free(self->humidity);
	//free(self->temperature);
	//free(self);
}

//actual task, methods devided so that it is possible to test
void humAndTempReader_executeTask(humAndTempReader_t self) {
	for (;;) {
		humAndTempReader_measure(self);
	}
}

void humAndTempReader_measure(humAndTempReader_t self) {//dummy
	EventBits_t uxBits = xEventGroupWaitBits(startMeasureEventGroup, //eventGroup
	BIT_MEASURE_HUMIDITY, //bits it is interested in
	pdTRUE, //clears the bits
	pdTRUE, //waits for both bits to be set
	portMAX_DELAY); //wait

	if ((uxBits & (BIT_MEASURE_HUMIDITY)) == (BIT_MEASURE_HUMIDITY)) {
		hih8120Meassure();
		if(hih8120IsReady())
		{
			self->humidity = hih8120GetHumidity();
			self->temperature = hih8120GetTemperature();
			printf("humidity and temperature done bit set");
		}
		vTaskDelay(2500); //pretend it takes some time

		//set done bit so that device knows meassurement is done
		xEventGroupSetBits(doneMeasuringEventGroup, BIT_DONE_MEASURE_HUMIDITY);
	}
}

float humAndTempReader_getHumidity(humAndTempReader_t self){
	return self->humidity;
}

float humAndTempReader_getTemperature(humAndTempReader_t self){
	return self->temperature;
}

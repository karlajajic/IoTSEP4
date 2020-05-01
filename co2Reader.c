#include <stdlib.h>
#include <stdint.h>
#include"co2Reader.h"

typedef struct co2reader co2reader;

typedef struct co2reader {
	int smth;
}co2reader;


co2reader_t co2Reader_create(void) {
	co2reader_t _new_reader = calloc(sizeof(co2reader), 1);
	if (_new_reader == NULL)
		return NULL;

	_new_reader->smth = 0;

	return _new_reader;
}
void co2Reader_destroy(co2reader_t self) {
	if (self == NULL)
		return;
	free(self->smth);
	free(self);
}

uint16_t co2Reader_getCO2(co2reader_t self) { //dummy
	int no = self->smth;
	no++;
	self->smth = no;
	return self->smth;
}
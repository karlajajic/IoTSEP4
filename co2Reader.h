#pragma once 
#include <stdint.h>

typedef struct co2reader* co2reader_t;

co2reader_t co2Reader_create(void);
void co2Reader_destroy(co2reader_t self);
uint16_t co2Reader_getCO2(co2reader_t self);
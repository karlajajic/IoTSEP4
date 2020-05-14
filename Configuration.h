
/*
 * Configuration.h
 *
 * Created: 14/05/2020 11:49:12
 *  Author: Haralambi
 */ 
typedef struct Configuration* configuration_t;

configuration_t configuration_create();
bool configuration_getDeviceON();
int configuration_getDeviceID();
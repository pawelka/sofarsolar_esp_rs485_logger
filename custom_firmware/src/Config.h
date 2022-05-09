#ifndef Config_h
#define Config_h



#include "Arduino.h"
#include "Debug.h"
#include <IotWebConf.h>
#include <IotWebConfUsing.h>

const char thingName[] = "sofarsolar2mqtt";
const char wifiInitialApPassword[] = "sofarsolar2mqtt";

#define CONFIG_VERSION "0001"
//#define CONFIG_PIN D2 //wemos
#define CONFIG_PIN 4 //esp07
#define STATUS_PIN LED_BUILTIN

#define STRING_LEN 128
#define NUMBER_LEN 32


char* getThingName();
boolean isNeedReset();
char* getMqttServerValue();
char* getMqttUserNameValue();
char* getMqttUserPasswordValue();
char* getMqttTopicValue();
int getCheckInterval();


void configSetup();
void configLoop();
boolean isOnline();



#endif

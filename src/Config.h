#ifndef Config_h
#define Config_h

#include "Arduino.h"
#include "Debug.h"
#include <IotWebConf.h>

const char thingName[] = "sofarsolar2mqtt";
const char wifiInitialApPassword[] = "sofarsolar2mqtt";

#define CONFIG_VERSION "sofarv1"
#define CONFIG_PIN D2
#define STATUS_PIN LED_BUILTIN

#define STRING_LEN 128


char* getThingName();
boolean isNeedReset();
char* getMqttServerValue();
char* getMqttUserNameValue();
char* getMqttUserPasswordValue();


void configSetup();
void configLoop();
boolean isOnline();



#endif
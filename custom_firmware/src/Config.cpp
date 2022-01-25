#include "Config.h"


// -- Callback method declarations.
void wifiConnected();
void configSaved();
boolean formValidator(iotwebconf::WebRequestWrapper* webRequestWrapper);

DNSServer dnsServer;
WebServer server(80);

// Include Update server
#ifdef ESP8266
# include <ESP8266HTTPUpdateServer.h>
#elif defined(ESP32)
# include <IotWebConfESP32HTTPUpdateServer.h>
#endif

// Create Update Server
#ifdef ESP8266
ESP8266HTTPUpdateServer httpUpdater;
#elif defined(ESP32)
HTTPUpdateServer httpUpdater;
#endif

char mqttServerValue[STRING_LEN];
char mqttUserNameValue[STRING_LEN];
char mqttUserPasswordValue[STRING_LEN];
char mqttTopicValue[STRING_LEN];
char checkIntervalValue[NUMBER_LEN];

boolean needReset = false;

IotWebConf iotWebConf(thingName, &dnsServer, &server, wifiInitialApPassword, CONFIG_VERSION);
iotwebconf::ParameterGroup mqttGroup = iotwebconf::ParameterGroup("mqttGroup");
iotwebconf::TextParameter  mqttServerParam = iotwebconf::TextParameter ("MQTT server", "mqttServer", mqttServerValue, STRING_LEN);
iotwebconf::TextParameter  mqttUserNameParam = iotwebconf::TextParameter ("MQTT user", "mqttUser", mqttUserNameValue, STRING_LEN);
iotwebconf::PasswordParameter mqttUserPasswordParam = iotwebconf::PasswordParameter("MQTT password", "mqttPass", mqttUserPasswordValue, STRING_LEN, "password");
iotwebconf::TextParameter  mqttTopicParam = iotwebconf::TextParameter ("MQTT topic", "mqttTopic", mqttTopicValue, STRING_LEN, "text", NULL, "inverter/status");
iotwebconf::NumberParameter checkIntervalParam = iotwebconf::NumberParameter ("Check inverval in sec", "checkInterval", checkIntervalValue, NUMBER_LEN, "300", "1..3600", "min='1' max='3600' step='1'");

boolean isNeedReset(){
    return needReset;
}
char* getMqttServerValue(){
    return mqttServerValue;
}
char* getMqttUserNameValue(){
    return mqttUserNameValue;
}
char* getMqttUserPasswordValue(){
    return mqttUserPasswordValue;
}
char* getMqttTopicValue(){
    return mqttTopicValue;
}
int getCheckInterval(){
    return atoi(checkIntervalValue);
}

void configSetup(){
  iotWebConf.skipApStartup();
  iotWebConf.setStatusPin(STATUS_PIN);
  iotWebConf.setConfigPin(CONFIG_PIN);
  mqttGroup.addItem(&mqttServerParam);
  mqttGroup.addItem(&mqttUserNameParam);
  mqttGroup.addItem(&mqttUserPasswordParam);
  mqttGroup.addItem(&mqttTopicParam);
  mqttGroup.addItem(&checkIntervalParam);
  iotWebConf.addParameterGroup(&mqttGroup);
  iotWebConf.setConfigSavedCallback(&configSaved);
  iotWebConf.setFormValidator(&formValidator);
  iotWebConf.setWifiConnectionCallback(&wifiConnected);
  iotWebConf.setupUpdateServer(
    [](const char* updatePath) { httpUpdater.setup(&server, updatePath); },
    [](const char* userName, char* password) { httpUpdater.updateCredentials(userName, password); });

  // -- Initializing the configuration.
  boolean validConfig = iotWebConf.init();
  if (!validConfig)
  {
    mqttServerValue[0] = '\0';
    mqttUserNameValue[0] = '\0';
    mqttUserPasswordValue[0] = '\0';
    mqttTopicValue[0] = '\0';
    checkIntervalValue[0] = '\0';
  }

  server.on("/", [] { iotWebConf.handleConfig(); });
  server.onNotFound([]() { iotWebConf.handleNotFound(); });


}

void configLoop(){
  iotWebConf.doLoop();

  if (needReset){
#ifndef DEBUG_DISABLED
    Debug.println("Rebooting after 1 second.");
#endif
    iotWebConf.delay(1000);
    ESP.restart();
  }

}

boolean isOnline(){
   return iotWebConf.getState() == iotwebconf::OnLine;
}

char* getThingName(){
   return iotWebConf.getThingName(); 
}

void configSaved()
{
#ifndef DEBUG_DISABLED
  Debug.println("Configuration was updated.");
#endif
  needReset = true;
}

boolean formValidator(iotwebconf::WebRequestWrapper* webRequestWrapper)
{
#ifndef DEBUG_DISABLED
  Debug.println("Validating form.");
#endif
  boolean valid = true;

  int l = server.arg(mqttServerParam.getId()).length();
  if (l < 3)
  {
    mqttServerParam.errorMessage = "Please provide at least 3 characters!";
    valid = false;
  }

  return valid;
}

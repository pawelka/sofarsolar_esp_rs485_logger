/**
 * Sofar Solar modbus2mqtt integration
 *
 * Copyright (C) 2018 Balazs Kelemen <prampec+arduino@gmail.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */
#include <Arduino.h>

#define HARDWARE_SERIAL

#include "Config.h"
#include "Debug.h"
#include "InverterValue.h"

#include <MQTT.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h>

WiFiClient net;
MQTTClient mqttClient(1500);
boolean skipUnknown = true;

boolean needMqttConnect = false;
unsigned long lastReport = 0;
unsigned long lastMqttConnectionAttempt = 0;

#ifndef HARDWARE_SERIAL
#include <SoftwareSerial.h> // Modbus RTU pins   D7(13),D8(15)   RX,TX
  SoftwareSerial serial2(D1, D2); //RX, TX
#endif
ModbusMaster node;

boolean connectMqtt();

const int INVERTER_BUFFER_TO_READ = 47;
const int INVERTER_VALUES_SIZE = 46;
const InverterValue INVERTER_VALUES[] = {
  InverterValue("status", -1, 1, ""), //pseudo value calcualted from fault message
  InverterValue("fault_message", 0, 1, ""),
  InverterValue("unknown_2", 1, 1, "?"),
  InverterValue("unknown_3", 2, 1, "?"),
  InverterValue("unknown_4", 3, 1, "?"),
  InverterValue("unknown_5", 4, 1, "?"),
  InverterValue("dc_voltage_1", 5, 10, "V"),
  InverterValue("dc_current_1", 6, 100, "A"),
  InverterValue("dc_voltage_2", 7, 10, "V"),
  InverterValue("dc_current_2", 8, 100, "A"),
  InverterValue("unknown_10", 9, 1, "?"),
  InverterValue("unknown_11", 10, 1, "?"),
  InverterValue("ac_power", 11, 0.1, "W"), //ac power
  InverterValue("unknown_13", 12, 1, "?"),
  InverterValue("ac_frequency", 13, 100, "Hz"),
  InverterValue("ac_voltage_1", 14, 10, "V"),
  InverterValue("ac_current_1", 15, 100, "A"),
  InverterValue("ac_voltage_2", 16, 10, "V"),
  InverterValue("ac_current_2", 17, 100, "A"),
  InverterValue("ac_voltage_3", 18, 10, "V"),
  InverterValue("ac_current_3", 19, 100, "A"),
  InverterValue("energy_total", 20, 2, 1, "kWh"), // total energy
  InverterValue("running_time", 22, 2, 1, "h"), //total operational time
  InverterValue("energy_today", 24, 100, "kWh"),
  InverterValue("unknown_26", 25, 1, "?"), //static value? during fault (no grid) stays the same
  InverterValue("temperature_module", 26, 1, "C"), //module temperature
  InverterValue("temperature_inverter", 27, 1, "C"), //inverter temperature
  InverterValue("bus_voltage", 28, 10, "V"), //bus voltage
  InverterValue("vice_cpu_input_voltage_1", 29, 10, "V"), // Vice CPU input voltage 1
  InverterValue("unknown_31", 30, 1, "?"), //during fault (no grid) there is an value
  InverterValue("countdown_timer", 31, 1, "h"), //count down timer (h)
  InverterValue("unknown_33", 32, 1, "?"),
  InverterValue("unknown_34", 33, 1, "?"), 
  InverterValue("unknown_35", 34, 1, "?"),
  InverterValue("pv1_insulation_resistance", 35, 1, "Ohm"), //PV1 insulation resistance
  InverterValue("pv2_insulation_resistance", 36, 1, "Ohm"), //PV2 insulation resistance
  InverterValue("isolation_impedance", 37, 1, ""), //isolation impedance - cathone to ground
  InverterValue("country_code?", 38, 1, ""), //country code 
  InverterValue("unknown_40", 39, 1, "?"), 
  InverterValue("unknown_41", 40, 1, "?"),
  InverterValue("unknown_42", 41, 1, "?"), //status ????
  InverterValue("phase_a_distribution", 42, 1, ""), //phase A distribution
  InverterValue("phase_b_distribution", 43, 1, ""), //phase B distribution
  InverterValue("phase_c_distribution", 44, 1, ""), //phase C distribution 
  InverterValue("unknown_46", 45, 1, "?"),
  InverterValue("unknown_47", 46, 1, "?")
};

RemoteDebug Debug;
int reportInterval;

void setup()
{
  Serial.begin(9600);
#ifndef DEBUG_DISABLED
  Debug.println("Starting up...");
#endif

  configSetup();

  mqttClient.begin(getMqttServerValue(), net);

#ifdef HARDWARE_SERIAL
 node.begin(1, Serial);
#else
  serial2.begin(9600);
  node.begin(1, serial2);
#endif
  delay(100);

  reportInterval = getCheckInterval() * 1000;

	Debug.begin("sofarsolar2mqtt"); // Initialize the WiFi server
  Debug.setResetCmdEnabled(true); // Enable the reset command
  Debug.showColors(false);
  

#ifndef DEBUG_DISABLED
  Debug.println("Ready.");
  Debug.print("Report interval: ");
  Debug.println(reportInterval);
#endif
}

char message[1500];
DynamicJsonDocument doc(1500);
int msgCount = 0;

void modbusLoop()
{
  unsigned long startTime = millis();
  if ((reportInterval < startTime - lastReport) && mqttClient.connected())
  {
    Debug.print("Report status with interval: ");
    Debug.println(reportInterval);


    doc.clear();
    node.clearResponseBuffer();
    int status = node.readHoldingRegisters(1, INVERTER_BUFFER_TO_READ);

    if (status == ModbusMaster::ku8MBSuccess){
      for(int idx = 0; idx < INVERTER_VALUES_SIZE; idx++){
        InverterValue curr = INVERTER_VALUES[idx];
        if(skipUnknown && curr.getUnit()[0] == '?'){
          continue;
        }
        if( idx <= 1){
          doc[curr.getValueName()] = curr.getStrValue(&node);
        }else{
          doc[curr.getValueName()] = curr.getValue(&node);
        }
      }

      msgCount++;
#ifndef DEBUG_DISABLED
      Debug.printf("Sending on MQTT channel '%s': %d \n", getMqttTopicValue(), msgCount);
#endif

      serializeJsonPretty(doc, message);
      mqttClient.publish(getMqttTopicValue(), message, true, 1);
    }

    lastReport = startTime;

#ifndef DEBUG_DISABLED
    Debug.printf("Loop time: %d \n", (millis() - startTime));
#endif    
  }
}

void loop()
{
  Debug.handle();
  // debugHandle(); // Equal to SerialDebug

  configLoop();
  mqttClient.loop();

  if (needMqttConnect)
  {
    if (connectMqtt())
    {
      needMqttConnect = false;
    }
  }
  else if ( isOnline() && !mqttClient.connected())
  {
#ifndef DEBUG_DISABLED
    Debug.println("MQTT reconnect");
#endif
    connectMqtt();
  }

  modbusLoop();
}

void wifiConnected()
{
  needMqttConnect = true;
}

boolean connectMqttOptions()
{
  boolean result;
  if (getMqttUserPasswordValue()[0] != '\0')
  {
    result = mqttClient.connect(getThingName(), getMqttUserNameValue(), getMqttUserPasswordValue());
  }
  else if (getMqttUserNameValue()[0] != '\0')
  {
    result = mqttClient.connect(getThingName(), getMqttUserNameValue());
  }
  else
  {
    result = mqttClient.connect(getThingName());
  }
  return result;
}

boolean connectMqtt()
{
  unsigned long now = millis();
  if (15000 > now - lastMqttConnectionAttempt)
  {
    // Do not repeat within 1 sec.
    return false;
  }
#ifndef DEBUG_DISABLED
  Debug.println("Connecting to MQTT server...");
#endif
  if (!connectMqttOptions())
  {
    lastMqttConnectionAttempt = now;
    return false;
  }
#ifndef DEBUG_DISABLED
  Debug.println("Connected!");
#endif

  return true;
}
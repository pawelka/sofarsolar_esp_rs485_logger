# ESPHome modbus

Here as a full config used by me to integrated with inverter. After that you see device in HA. 

Config utilize ESPHome modbus feature https://esphome.io/components/modbus.html

Any suggestion how to improve config are welcome. Potencial improvements
* better support for fault code. Right now it's a number on;y

## Config

```
substitutions:
  devicename: sofarsolar-logger
  friendly_name: SofarSolar

wifi:
  ssid: !secret wifi-ssid
  password: !secret wifi-password
  fast_connect: true

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "sweet-fallback-${devicename}"
    password: !secret wifi-fallback-password

captive_portal:

# Enable Home Assistant API
api:
  password: !secret api-password

ota:
  password: !secret ota-password

# Enable Web server
web_server:
  port: 80

esphome:
  name: $devicename
  platform: ESP8266
  board: esp01_1m
  board_flash_mode: dout

logger:
    level: INFO
    baud_rate: 0
    
uart:
  id: mod_bus
  tx_pin: 1
  rx_pin: 3
  baud_rate: 9600
  stop_bits: 1

modbus:
  id: mod_bus_sofar

modbus_controller:
  - id: sofarsolar
    address: 0x01
    modbus_id: mod_bus_sofar
    update_interval: 10s

# Report basic device information like it's Wi-Fi signal strength and uptime
sensor:
  - platform: wifi_signal
    id: inverter_wifi_signal
    name: ${friendly_name} WiFi Signal
    update_interval: 60s
  - platform: uptime
    id: inverter_uptime
    name: ${friendly_name} Uptime
    filters:
      - lambda: return x / 60.0;
    unit_of_measurement: minutes
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} Fault Code
    id: inverter_fault_code
    register_type: holding
    address: 0x0001
    value_type: U_WORD
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} DC1 Voltage
    id: inverter_dc_v1
    register_type: holding
    address: 0x0006
    unit_of_measurement: "V"
    icon: "mdi:alpha-v-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 1
    filters:
      - multiply: 0.1
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} DC1 Current
    id: inverter_dc_c1
    register_type: holding
    address: 0x0007
    unit_of_measurement: "A"
    icon: "mdi:alpha-a-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 2
    filters:
      - multiply: 0.01
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} DC2 Voltage
    id: inverter_dc_v2
    register_type: holding
    address: 0x0008
    unit_of_measurement: "V"
    icon: "mdi:alpha-v-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 1
    filters:
      - multiply: 0.1
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} DC2 Current
    id: inverter_dc_c2
    register_type: holding
    address: 0x0009
    unit_of_measurement: "A"
    icon: "mdi:alpha-a-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 2
    filters:
      - multiply: 0.01
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} AC Power
    id: inverter_ac_power
    register_type: holding
    address: 0x000c
    unit_of_measurement: "W"
    device_class: "power"
    value_type: U_WORD
    filters:
      - multiply: 10
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} AC Freq
    id: inverter_ac_freq
    register_type: holding
    address: 0x000e
    unit_of_measurement: "Hz"
    icon: "mdi:current-ac"
    value_type: U_WORD
    accuracy_decimals: 2
    filters:
      - multiply: 0.01
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} AC1 Voltage
    id: inverter_ac_v1
    register_type: holding
    address: 0x000f
    unit_of_measurement: "V"
    icon: "mdi:alpha-v-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 1
    filters:
      - multiply: 0.1
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} AC1 Current
    id: inverter_ac_c1
    register_type: holding
    address: 0x0010
    unit_of_measurement: "A"
    icon: "mdi:alpha-a-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 2
    filters:
      - multiply: 0.01
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} AC2 Voltage
    id: inverter_ac_v2
    register_type: holding
    address: 0x0011
    unit_of_measurement: "V"
    icon: "mdi:alpha-v-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 1
    filters:
      - multiply: 0.1
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} AC2 Current
    id: inverter_ac_c2
    register_type: holding
    address: 0x0012
    unit_of_measurement: "A"
    icon: "mdi:alpha-a-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 2
    filters:
      - multiply: 0.01
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} AC3 Voltage
    id: inverter_ac_v3
    register_type: holding
    address: 0x0013
    unit_of_measurement: "V"
    icon: "mdi:alpha-v-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 1
    filters:
      - multiply: 0.1
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} AC3 Current
    id: inverter_ac_c3
    register_type: holding
    address: 0x0014
    unit_of_measurement: "A"
    icon: "mdi:alpha-a-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 2
    filters:
      - multiply: 0.01
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} Energy total
    id: inverter_energy_total
    register_type: holding
    address: 0x0015
    unit_of_measurement: "kWh"
    device_class: "power"
    value_type: U_DWORD
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} Running time
    id: inverter_running_time
    register_type: holding
    address: 0x0017
    unit_of_measurement: "h"
    value_type: U_DWORD
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} Energy today
    id: inverter_energy_today
    register_type: holding
    address: 0x0019
    unit_of_measurement: "kWh"
    device_class: "power"
    value_type: U_WORD
    accuracy_decimals: 2
    filters:
      - multiply: 0.01
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} Temprature module
    id: inverter_temp_module
    register_type: holding
    address: 0x001B
    unit_of_measurement: "°C"
    device_class: "temperature"
    value_type: U_WORD
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} Temprature inverter
    id: inverter_temp_inverter
    register_type: holding
    address: 0x001C
    unit_of_measurement: "°C"
    device_class: "temperature"
    value_type: U_WORD
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} Bus voltage
    id: inverter_bus_voltage
    register_type: holding
    address: 0x001D
    unit_of_measurement: "V"
    icon: "mdi:alpha-v-circle-outline"
    value_type: U_WORD
    accuracy_decimals: 1
    filters:
      - multiply: 0.1
  - platform: modbus_controller
    modbus_controller_id: sofarsolar
    name: ${friendly_name} Countdown timer
    id: inverter_countdown_timer
    register_type: holding
    address: 0x0020
    unit_of_measurement: "h"
    value_type: U_WORD

      
# Basic switch to allow you to restart the device remotely
switch:
  - platform: restart
    name: ${friendly_name} Restart

binary_sensor:
  - platform: status
    name: ${friendly_name} Status
```

## Image
![](../images/esphome_ha.png)

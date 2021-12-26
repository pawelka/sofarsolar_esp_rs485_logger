# SofarSolar ESP RS485 logger to MQTT

## Background
From the shelf logger is only works with SolarMan cloud, but I wanted to have full local integration with Home Assistant, because of poor internet connection. Additionally refresh interval from orginal logger (I managed to integrate with Home Assistant too, see HA Add-One project) is not synchronized with energy meter sensor which checks consumption and production from/to grid. The result was weird, because power wheel shows that house produced energy. 

I asked SofarSolar for documentation of RS485 communication, but no lack. I managed to read register based on old SofarSolar documentation for other inverters models other than KTL-X (my inverter) and reversed engineered data based on information sent to SolarMan cloud. 

At the end I finished with two working loggers in parallel ;-)

## Supported inverters
For sure SofarSolar KTL-X 8.8 and other powers from KTL-X. There is a chance that other brands from the same familly with work, so if someone check please let me know.

Tested supported devices:
* Sofar KTL-X 8.8 (whole series of KTL-X should work)
* Sofar 4K TLM-G2 

## Parts needed to build own local logger
* "ESP-07 WiFi serial" - you will find the part under this name
* RS485 to TTL converter compatible with 3v3 (3v3 is an important, because ESP-07 operates on 3v3 TTL levels)
* External antena with connector
* RJ45 connector and cables

I bought everything from China directly. Total cost is around $5, so much cheaper than orignal logger and more freedom with integration.

## Schematic
![](images/schematic.jpg)

## Firmware choice 

My first approach was to write custom firmware, because I couldn't find any other solution, but overtime I realized that there can by some other solutions which can integrate with modbus like Tasmota or ESPHome. With Tasmota and ESPHome the same goal can be achived. In this project you can find all solutions that I tried:

* [Custom firmware](custom_firmware)
* [Tasmota SMI script](tasmota_smi)
* [ESPHome configuration](esphome_config)

I prefer ESPHome script, because of seamless integration with ESPHome including correct support for icon and device class.

Custom software is ok only for my use, because it's dedicated to my setup and my hardware, so I see some issues and it's hard to maintain this solution, comparing to Tasmota and ESPHome supports where there is huge community to support it.

Tasmota SMI script is not fully tested, but I'm leaving script maybe someone wants to use it for some other integrations for example Domoticz (my goal was HomeAssistant integration)

## Images
### Prototype
![](images/prototype.jpg)
### Final solution
![](images/desk1.jpg)
![](images/desk2.jpg)
![](images/inverter1.jpg)
![](images/inverter2.jpg)

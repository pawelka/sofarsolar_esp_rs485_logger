# SofarSolar ESP RS485 logger to MQTT

## Background
From the shelf logger is only works with SolarMan cloud, but I wanted to have full local integration with Home Assistant, because of poor internet connection. Additionally refresh interval from orginal logger (I managed to integrate with Home Assistant too, see HA Add-One project) is not synchronized with sensor with I have for checking energy consumption and production from/to grid. The result was funny, because power wheel shows that house produced energy. 

I asked SofarSolar for documentation of RS485 communication, but no lack. I managed to read register based on old SofarSolar documentation for other inverters models other than KTL-X (my inverter) and reversed engineered data based on information sent to SolarMan cloud. 

At the end I finished with two working loggers in parallel ;-)

## Supported inverters
For sure SofarSolar KTL-X 8.8 and other powers from KTL-X. There is a chance that other brands from the same familly with work, so if someone check please let me know.

## Parts needed to build own local logger
* "ESP-07 WiFi serial" - you will find the part under this name
* RS485 to TTL converter compatible with 3v3 (3v3 is an important, because ESP-07 operates on 3v3 TTL levels)
* External antena with connector
* RJ45 connector and cables

I bought everything from China directly. Total cost is around $5, so much cheaper than orignal logger and more freedom with integration.

## Schematic
![](images/schematic.jpg)

## Image installation
Download image from *build* folder and load it to your ESP (I used esptool to do this). There is a lot of tutorials how to do it on the internet ;-)

## Connection to the inverter
Check last image. I used 5V power directly from the inverter. Red cable on photo is VCC and brown is GND. Inverter provides power only during a day, but solutions uses MQTT, so during the night there is no messages.

## Configuration
Solution uses IotWebConf library, so after uploading image and switching ESP to boot mode, you should see access point to which you can connect and configure your ESP RS485 logger.

Default IP 192.168.4.1
Username: admin
Password: sofarsolar2mqtt

All available parameters are on screen below:

![](images/config.png)

## Images
### Prototype
![](images/prototype.jpg)
### Final solution
![](images/desk1.jpg)
![](images/desk2.jpg)
![](images/inverter1.jpg)
![](images/inverter2.jpg)







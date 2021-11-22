# Weather Station

This project has two purposes, the first and primary purpose is to build a small device that can be deployed to gather
and optionally display atmospheric temperature, humidity, and barometric pressure both indoors and out. The second
purpose is to serve as a simple test-bed for microcontroller platforms, wireless connectivity solutions, and software
development options. To get started quickly I'm using an ESP32 communicating over WiFi leveraging the Arduino framework,
both the ESP32 and WiFi are way more power hungry than would be ideal for a battery powered version but for indoor use
with wall power it will be fine.

## Microcontrollers to try
These options represent a few different families of technology, some will be much lower power than the base ESP32 while
others will be about the same.

* ESP32-C3 (Risc-V single core)
* STM32 line (probably the F0)
* ???

## Wireless technologies to explore
WiFi is great for when a ton of bandwidth is needed and the requisite power is available. Eventually I would like to
build a solar-powered version of this I can stick out in the yard or even in the forest so a less power hungry, 
longer-range solution will be nice to have.

* LoRa
* Zigbee
* Thread

## Licensing
As of right now the code is licensed under the LGPL since that's what the ESP32's Arduino stack is licensed under, if
another framework is used for other implementations the licensing will probably be changed to something less restrictive
of derivative works.

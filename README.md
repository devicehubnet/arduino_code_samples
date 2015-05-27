# Arduino Code Samples 
Sample code for Arduino based development boards for Internet of Things applications using devicehub.net platform.

DeviceHub.net is an IoT platform that bridges the gap between hardware devices and mobile/web developers by providing a collection of straightforward APIs to both ends. The web platform, not only gathers data but makes sense of the data spewed out by billions of connected devices and helps people make smarter decisions by giving them the possibility to remote control the devices.

# Documentation
First open the PubSubClient.h library header file and make the following 
modification in that file. (increase the packet size as shown below). 

Replace 128 with 256 at this line

define MQTT_MAX_PACKET_SIZE 128

define MQTT_MAX_PACKET_SIZE 256

# Requirements
PubSubClient library https://github.com/knolleary/pubsubclient


# Contributing
Ideas, bugs, tests and pull requests always welcome.

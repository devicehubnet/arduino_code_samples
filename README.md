# Arduino Code Samples 
MQTT code samples for Arduino Ethernet (Shield)

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
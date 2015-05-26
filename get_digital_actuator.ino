/*
DeviceHUB.net sample code for getting digital actuator state.

In this example an LED is attached to digital pin 5 on the Arduino Ethernet Board.

Download and install PubSubClient library from https://github.com/knolleary/pubsubclient

First open the PubSubClient.h library header file and make the following 
modification in that file. (increase the packet size as shown below). 

// MQTT_MAX_PACKET_SIZE : Maximum packet size
//#define MQTT_MAX_PACKET_SIZE 128
#define MQTT_MAX_PACKET_SIZE 256

created 26 May 2015
by Alexandru Gheorghe

*/

#include <SPI.h>    
#include <Ethernet.h>                            
#include <PubSubClient.h>

// MAC Address of Arduino Ethernet (Sheild)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#define API_KEY         "paste_your_API_KEY_here"
#define PROJECT_ID      "paste_your_PROJECT_ID_here"
#define ACTUATOR_NAME   "paste_your_ACTUATOR_NAME_here"
#define DEVICE_UUID     "paste_your_DEVICE_UUID_here"
#define sec             1000
// Digital pin 5 is the LED output pin
#define LED             5

char clientId[]          = "Arduino_Ethernet";
char actuatorTopic[]     = "/a/"API_KEY"/p/"PROJECT_ID"/d/"DEVICE_UUID"/actuator/"ACTUATOR_NAME"/state";
// server mqtt.devicehub.net ip
char server[]            = "104.155.7.31";
char message_buffer[150];

EthernetClient apiClient;

PubSubClient client(server, 1883, callback, apiClient);

// handles message arrived on subscribed topic
void callback(char* topic, byte* payload, unsigned int length)
{
  int i = 0;
  
  // create buffer with null terminator
  for(i=0; i<length; i++) 
  {
    message_buffer[i] = payload[i];
  }
  message_buffer[i] = '\0';
  
  String msgString = String(message_buffer).substring(43,44);
  
  Serial.println("message arrived: " + msgString);
  
  if(msgString == "0")
  { 
    //if message received is 0, LED is turned off
    digitalWrite(LED,LOW);
  }
  else 
  {
    //if message received is 1, LED is turned on
    digitalWrite(LED,HIGH);
  }

}

void setup(void)  
{
  // init serial for debugging
  Serial.begin(9600);
  
  //initialize the digital pin as an output.
  pinMode(LED, OUTPUT);
  // light should be off
  digitalWrite(LED,LOW);
  
  if (Ethernet.begin(mac) == 0)  
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac);
  }

  Serial.println("\nStarting connection to server...");
  
  if(client.connect(clientId))  
  {
    client.subscribe(actuatorTopic);
    Serial.println("Successfuly connected and running!");
  }
  else 
  {
    Serial.println("Connection problem");
  }

}

void loop(void)  
{
  //if client it's not connected or disconnects here we try to reconnect
  if (!client.connected())
  { 
    Serial.println("reconnecting ...");
    client.connect(clientId);
    delay(3*sec); 
    client.subscribe(actuatorTopic);
  }
  // MQTT client loop processing
  client.loop();

}
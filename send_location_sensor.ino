/*
DeviceHUB.net sample code for sending location point.

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

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#define API_KEY         "paste_your_API_KEY_here"
#define PROJECT_ID      "paste_your_PROJECT_ID_here"
#define SENSOR_NAME  "paste_your_ACTUATOR_NAME_here"
#define DEVICE_UUID     "paste_your_DEVICE_UUID_here"
#define sec             1000

char clientId[]       = "Arduino_Ethernet";
char sensorTopic[]    = "/a/"API_KEY"/p/"PROJECT_ID"/d/"DEVICE_UUID"/sensor/"SENSOR_NAME"/data";
// server mqtt.devicehub.net
char server[]            = "mqtt.devicehub.net";
char message_buffer[250];

EthernetClient apiClient;

void callback(char* topic, byte* payload, unsigned int length)
{
  // handle message arrived
}

PubSubClient client(server, 1883, callback, apiClient);

boolean conn = false;
unsigned long time;

void setup(void)
{
  // init serial for debugging
  Serial.begin(9600);
  
  if (Ethernet.begin(mac) == 0)  
  {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac);
  }

  Serial.println("\nStarting connection to server...");
  
  if(client.connect(clientId))  
  {
    conn = true;
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
    sendData();  
  }

  sendData();
}

void sendData(void) 
{
  //simulation for GPS sensor
  String longitude = "44.436069";
  String latitude  = "26.133531";
  
  if(conn) 
  {
    //publishing new position every 5 seconds
    if (millis() > (time + 5000)) 
    {
      time = millis();  
      
      String pubString = "{\"value\": {\"type\": \"Feature\", \"properties\": 
      {}, \"geometry\": { \"type\": \"Point\", \"coordinates\": [" + 
      latitude + ", " + longitude + "] } } }";
      
      pubString.toCharArray(message_buffer, pubString.length()+1);
    
      Serial.println("Publishing new point: ");
      Serial.println("Latitude: " + latitude + " Longitude: " + longitude);
      
      client.publish(sensorTopic, message_buffer);
    }
    // MQTT client loop processing
    client.loop();
  }

}
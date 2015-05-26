/*
DeviceHUB.net sample code for sending two strings.

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
#define STRING_NAME1    "paste_your_first_STRING_NAME_here"
#define STRING_NAME2    "paste_your_second_STRING_NAME_here"
#define DEVICE_UUID     "paste_your_DEVICE_UUID_here"
#define sec             1000

char clientId[]       = "Arduino_Ethernet";
char sensorTopic1[]    = "/a/"API_KEY"/p/"PROJECT_ID"/d/"DEVICE_UUID"/sensor/"STRING_NAME1;
char sensorTopic2[]    = "/a/"API_KEY"/p/"PROJECT_ID"/d/"DEVICE_UUID"/sensor/"STRING_NAME2;
// server mqtt.devicehub.net ip
char server[]         = "104.155.7.31";
char message_buffer1[250];
char message_buffer2[250];

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
  //simulation for two strings
  String str1 = "ThisIsATestSring";
  String str2 = "123456789";
    
  if(conn) 
  {
    //publishing new string every 5 seconds
    if (millis() > (time + 5000)) 
    {
      time = millis();
      
      String pubString1 = "{\"value\": \"" + str1 + "\"}";
      String pubString2 = "{\"value\": \"" + str2 + "\"}";

      pubString1.toCharArray(message_buffer1, pubString1.length()+1);
      pubString2.toCharArray(message_buffer2, pubString2.length()+1);
    
      Serial.print("Publishing new strings: ");
      Serial.print(str1);
      Serial.print(" and ");
      Serial.println(str2);
      
      client.publish(sensorTopic1, message_buffer1);
      client.publish(sensorTopic2, message_buffer2);
    }
    // MQTT client loop processing
    client.loop();
  }

}

/*
DeviceHUB.net sample code for sending an digital sensor and an analog sensor.

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
#define AN_SENSOR_NAME  "paste_your_SENSOR_NAME_here"
#define DI_SENSOR_NAME  "paste_your_SENSOR_NAME_here"
#define DEVICE_UUID     "paste_your_DEVICE_UUID_here"
#define sec             1000

char clientId[]          = "Arduino_Ethernet";
char an_sensorTopic[]    = "/a/"API_KEY"/p/"PROJECT_ID"/d/"DEVICE_UUID"/sensor/"DI_SENSOR_NAME"/data";
char di_sensorTopic[]    = "/a/"API_KEY"/p/"PROJECT_ID"/d/"DEVICE_UUID"/sensor/"AN_SENSOR_NAME"/data";
// server mqtt.devicehub.net ip
char server[]         = "104.155.7.31";
char message_buff_an[20];
char message_buff_di[10];

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
  //simulation for analog sensor and digital sensor 
  unsigned int analog_sensor = random(0, 2);
  float digital_sensor = random(0, 1023.0);

  if(conn) 
  {
    //publishing new value every 5 seconds
    if (millis() > (time + 5000)) 
    {
      time = millis();
      
      String pubString_an = "{\"value\": " + String(analog_sensor) + "}";
      String pubString_di = "{\"value\": " + String(digital_sensor) + "}";

      pubString_an.toCharArray(message_buff_an, pubString_an.length()+1);
      pubString_di.toCharArray(message_buff_di, pubString_di.length()+1);
      
      Serial.print("Publishing new value: ");
      Serial.print(analog_sensor);
      Serial.print(" and ");  
      Serial.println(digital_sensor);

      client.publish(an_sensorTopic, message_buff_an);
      client.publish(di_sensorTopic, message_buff_di);
    }
    // MQTT client loop processing
    client.loop();
  }

}
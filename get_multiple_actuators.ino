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
//add or remove based on the number of actuators
#define ACTUATOR1_NAME   "paste_your_ACTUATOR_NAME_here"
#define ACTUATOR2_NAME   "paste_your_ACTUATOR_NAME_here"
#define ACTUATOR3_NAME   "paste_your_ACTUATOR_NAME_here"
#define DEVICE_UUID     "paste_your_DEVICE_UUID_here"
#define sec             1000

char clientId[]          = "Arduino_Ethernet_test_actx2";
//add or remove based on the number of actuators
char actuator1Topic[]     = "/a/"API_KEY"/p/"PROJECT_ID"/d/"DEVICE_UUID"/actuator/"ACTUATOR1_NAME"/state";
char actuator2Topic[]     = "/a/"API_KEY"/p/"PROJECT_ID"/d/"DEVICE_UUID"/actuator/"ACTUATOR2_NAME"/state";
char actuator3Topic[]     = "/a/"API_KEY"/p/"PROJECT_ID"/d/"DEVICE_UUID"/actuator/"ACTUATOR3_NAME"/state";
//Devicehub.net MQTT server
char server[]            = "mqtt.devicehub.net";
char message_buffer[150];

EthernetClient apiClient;

PubSubClient client(server, 1883, callback, apiClient);

// handles message arrived on subscribed topic
void callback(char* topic, byte* payload, unsigned int length)
{
  // handles extracting the actuator value from payload
  int i=0;
  int j=0;
  int k=0;
  for(i=0; i<length; i++){
    message_buffer[i] = payload[i];
  }
  message_buffer[i] = '\0';
  for(i=0; i<length; i++){
    if(!(strncmp(&message_buffer[i], "s",1) || strncmp(&message_buffer[i+3], "t", 1))){
      break;
    }
  }
  for(j=i+9; j<length; j++){
    if(!(strncmp(&message_buffer[j], "\"",1))){
      break;
    }
  }
  String string_actuator_value = "";
  for(k=i+9; k<j; k++){
    string_actuator_value += message_buffer[k];
  }
  int actuator_value = string_actuator_value.toInt();
  
  // here is the value from the actuator
  Serial.print(topic);
  Serial.print(" value: ");
  Serial.println(actuator_value);
  
  // you can use it here accordingly just add or remove based on how manny actuators you have
  if(String(topic) == String(actuator1Topic)){
    //DO actuator1 action
  }else if ((String(topic) == String(actuator2Topic))){
    //DO actuator2 action
  }else if ((String(topic) == String(actuator3Topic))){
    //DO actuator3 action
  }
  
}

void setup(void)  
{
  // init serial for debugging
  Serial.begin(9600);
  
  if (Ethernet.begin(mac) == 0){
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac);
  }

  Serial.println("\nStarting connection to server...");
  
  if(client.connect(clientId)){
    //add or remove based on the number of actuators
    client.subscribe(actuator1Topic);
    client.subscribe(actuator2Topic);
    client.subscribe(actuator3Topic);
    Serial.println("Successfuly connected and running!");
  }else{
    Serial.println("Connection problem");
  }
}

void loop(void)  
{
  //if client it's not connected or disconnects here we try to reconnect
  if (!client.connected()){ 
    Serial.println("reconnecting ...");
    client.connect(clientId);
    delay(3*sec); 
    //add or remove based on the number of actuators
    client.subscribe(actuator1Topic);
    client.subscribe(actuator2Topic);
    client.subscribe(actuator3Topic);
  }
  // MQTT client loop processing
  client.loop();
}

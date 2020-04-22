
/*
Author  : namira Fathani
Date    : 21/04/2020
*/


//==========================================================================================================================================//
//===================================================|   Initialization Program    |========================================================//                                         
//==========================================================================================================================================//
#include <RTClib.h>
#include <Wire.h>
#include <Ethernet.h>                                               // Add library ethernet
#include <SPI.h>                                                    // Add library protocol communication SPI
#include <ArduinoJson.h>                                            // Add library arduino json 
#include <PubSubClient.h>                                           // Add library PubSubClient MQTT

#define DEBUG

#define limitData 60000
#define timer1 5000
#define timer2 10000

#define COM1 32
#define COM2 33

RTC_DS1307 RTC;

/* configur etheret communication */
byte mac[]  = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };                // MAC Address by see sticker on Arduino Etherent Shield or self determine
IPAddress ip(192, 168, 0, 110);                                     // IP ethernet shield assigned, in one class over the server
IPAddress server(192, 168, 0, 106);                                 // IP LAN (Set ststic IP in PC/Server)
int portServer = 1883;                                              // Determine portServer MQTT connection

EthernetClient ethClient;                                           // Instance of EthernetClient is a etcClient
PubSubClient client(ethClient);                                     // Instance of client ia a client

//==========================================================================================================================================//
//==========================================================|   Procedure callback    |=====================================================//                                         
//==========================================================================================================================================//
char data[80];
StaticJsonBuffer<300> jsonBuffer;
void callback(char* topic, byte* payload, unsigned int length){
  #ifdef DEBUG
  Serial.print("message arrive in topic");
  Serial.println(topic);
  Serial.print("Message");
  #endif

  char inData[500];
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
    inData[(i-0)] = (const char*)payload[i];
  }

  #ifdef DEBUG
  Serial.println();
  Serial.println("------------------");
  #endif

  JsonObject& root = jsonBuffer.parseObject(inData);
  String time = root["current_time"];
  int statusTime = root["flagtime"];

  int serverLastMAC01 = root["MAC01"];
  int serverLastMAC02 = root["MAC02"];
  int flagreply = root["flagreply"];

  #ifdef DEBUG
  Serial.print("serverLastMAC01="); Serial.println(serverLastMAC01);
  Serial.print("serverLastMAC02="); Serial.println(serverLastMAC02);
  Serial.print("flagreply="); Serial.println(flagreply);
  #endif
}


//==========================================================================================================================================//
//=========================================================|   Procedure reconnect    |=====================================================//                                         
//==========================================================================================================================================//
void reconnect(){
  while(!client.connected()){
    #ifdef DEBUG
    Serial.print("Attemping MQTT connection...");
    #endif
    if(client.connect("ethernetClient")){
      Serial.println("connected");
      // Publish variable startup system
      const size_t restart = JSON_OBJECT_SIZE(2);
      DynamicJsonBuffer jsonBuffer(restart);
      JsonObject& root = jsonBuffer.createObject();
      
      root["id_controller"] = "CTR01";
      root["flagstart"] = 1;

      char buffermessage[300];
      root.printTo(buffermessage, sizeof(buffermessage));

      #ifdef DEBUG
      Serial.println("Sending message to MQTT topic...");
      Serial.println(buffermessage);
      #endif

      client.publish("PSI/countingbenang/datacollector/startcontroller", buffermessage);
      
      if (client.publish("PSI/countingbenang/datacollector/startcontroller", buffermessage) == true){
        #ifdef DEBUG
        Serial.println("Succes sending message");
        Serial.println("--------------------------------------------");
        Serial.println("");
        #endif
      } else {
      
      #ifdef DEBUG
      Serial.println("Error publishing");
      Serial.println("--------------------------------------------");
      Serial.println("");
      #endif
      }
    }
    else{

      #ifdef DEBUG
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again 5 second");
      #endif
      delay(2000);
    }
  }
}


// //==========================================================================================================================================//
// //============================================|   Procedure publish data to app server  |===================================================//                                         
// //==========================================================================================================================================//
/* publish data sensor 1 */
void publishData_S1(){
  #ifdef DEBUG
  Serial.print(12);                                                                // line debugging
  Serial.print("|substraction|");                                                       // line debugging
  Serial.println(0);                                                           // line debugging
  #endif
  

/* ArduinoJson create jsonDoc 
Must be know its have a different function 
if you use library ArduinoJson ver 5.x.x or 6.x.x
-- in this program using library ArduinoJson ver 5.x.x
*/
const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(9);                                         // define number of key-value pairs in the object pointed by the JsonObject.

 DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);                                             // memory management jsonBuffer which is allocated on the heap and grows automatically (dynamic memory)
  JsonObject& JSONencoder = jsonBuffer.createObject();                                  // createObject function jsonBuffer

  /* Encode object in jsonBuffer */
  // JSONencoder["id_controller"] = "CTR01";                                               // key/object its = id_controller
  // JSONencoder["id_machine"] = id_machine1;                                              // key/object its = id_machine
  // JSONencoder["date"] = Stringyear1 +"-"+Stringmonth1+"-"+Stringday1;
  // JSONencoder["time"] = Stringhour1+":"+Stringminute1;
  // JSONencoder["clock"] = Stringyear1 +"-"+Stringmonth1+"-"+Stringday1+" "+Stringhour1+":"+Stringminute1;
  // JSONencoder["count"] = data_S1;                                                    // key/object its = count
  // // JSONencoder["length"] = ropelength;                                                  // key/object its = length
  // // JSONencoder["length"] = 511.1;                                                        // key/object its = length for debbuging
  // JSONencoder["status"] = status_S1;                                                            // key/object its = status
  // JSONencoder["flagsensor"] = 1;                                                            // key/object its = limit

  /* when use dummy data publish */
  JSONencoder["id_controller"] = "CTR01";                                               // key/object its = id_controller
  JSONencoder["id_machine"] = "MAC01_01";                                              // key/object its = id_machine
  JSONencoder["date"] = "2020-02-02";
  JSONencoder["clock"] = "02:02";
  JSONencoder["time"] = "2020-02-02 02:02";
  JSONencoder["count"] = 12;                                                    // key/object its = count
  JSONencoder["length"] = 602.88;                                                        // key/object its = length for debbuging
  JSONencoder["status"] = 0;                                                            // key/object its = status
  JSONencoder["flagsensor"] = 1;                                                            // key/object its = limit

  char JSONmessageBuffer[500];                                                          // array of char JSONmessageBuffer is 500
  JSONencoder.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));                    // “minified” a JSON document

  #ifdef DEBUG
  Serial.println("Sending message to MQTT topic...");                                   // line debugging
  Serial.println(JSONmessageBuffer);                                                    // line debugging
  #endif

  client.publish("PSI/countingbenang/datacollector/reportdata", JSONmessageBuffer);     // publish payload to broker <=> client.publish(topic, payload);

  /* error correction */
  if(client.publish("PSI/countingbenang/datacollector/reportdata", JSONmessageBuffer) == true){
    #ifdef DEBUG
    Serial.println("Success sending message");
    Serial.println("--------------------------------------------");
    Serial.println("");
    #endif
  } else {
    #ifdef DEBUG
    Serial.println("Error Sending");
    Serial.println("--------------------------------------------");
    Serial.println("");
    #endif
  }
}


//==========================================================================================================================================//
//=======================================================|   Setup Program    |=============================================================//                                         
//==========================================================================================================================================//
void setup() {
 /* Setup Ethernet connection */
  Ethernet.begin(mac, ip);
  /* Setup Broker (server) MQTT Connection */
  client.setServer(server, portServer);
  // client.setCallback(callback);

  // initialize serial baudrate
  Serial.begin(9600);                                           

  reconnect();
  client.subscribe("PSI/countingbenang/server/infotimestamp");    //topic get data timestamp from server
  client.subscribe("PSI/countingbenang/tablet/setreset");    //topic get reset from server
}


//==========================================================================================================================================//
//=====================================================|   Main Loop Program    |===========================================================//                                         
//==========================================================================================================================================//
void loop() {
  // put your main code here, to run repeatedly:
  publishData_S1();
  delay(3000);
}

// ***** main.cpp *****
// Libraries

#include <SoftwareSerial.h>
#include <SPI.h>
#include <Time.h>
#include <TimeAlarms.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "credentials.h"
#include <HardwareSerial.h>

#define MAIN_CORE 1
#define TASK_CORE 0

//======================================================================//
// Global Variables

// *** SIM ***
#ifndef CREDENTIALS_H
  char dest_phone_no[16] = "+000000000000";
#endif

// *** MQTT ***
WiFiClient client;
PubSubClient mqtt(client);

/****************************** Feeds ***************************************/

const char* commands_topic = "waterbox/W0001/commands";
const char* flow_sensor_topic = "waterbox/W0001/flow_sensor";
const char* temp_sensor_topic = "waterbox/W0001/temp_sensor";
const char* test_topic = "waterbox/W0001/test";

//======================================================================//
// Headers

#include "serial.h" 
#include "flowsensor.h"
#include "sim.h"
#include "HelperTasks.h"
// #include "alarms.h" // *** THIS FILE IS NOT YET USED ***
// #include "rtc.h" // *** THIS FILE IS NOT YET USED ***
// #include "sdcard.h" // *** THIS FILE IS NOT YET USED ***

//======================================================================//
// Main

void setupWifi();
void setupMqtt();
void callback(char*, byte*, unsigned int);
void reconnect();

void setup()
{
  // ******* Setup_Start *******

  setupSerial();
  setupEeprom();
  setupSim();
  setupFlowSensor();
  setupWifi();
  setupMqtt();
  // setupRTC();
  // setupSDCard();
  
  // ******* Setup_End *******

  setTime(22,15,30,23,1,21); // set initial time, can be fixed later
  Alarm.timerRepeat(DEBIT_COUNT_INTERVAL, printdebitCountTemporary); // turn on repeater to count debit
  Alarm.timerRepeat(DEBIT_REPORT_INTERVAL, reportdebitCountTemporary); // turn on repeater to publish debit to MQTT Broker
  
  Serial.println("Setup: Initialization done.");
}

void loop() 
{
  echo();
  Alarm.delay(1); // wait one second between clock display. MUST BE TURNED ON TO USE Alarm::timerRepeater
}

//======================================================================//
// Mqtt & Wifi

void setupMqtt() {
  Serial.println("Setup: Initializing connection to MQTT broker");
  mqtt.setServer(MQTT_SERVER, 1883);
  mqtt.setCallback(callback);
  while (!mqtt.connect(MQTT_CLIENTNAME, MQTT_USERNAME, MQTT_KEY));
  Serial.println("Setup: MQTT broker connected!");

  Serial.print("Setup: Executing on core ");
  Serial.println(xPortGetCoreID());
  Serial.print("Setup: priority = ");
  Serial.println(uxTaskPriorityGet(NULL));

  xTaskCreatePinnedToCore(
    TaskHeartbeatTestPublish
    ,  "TaskHeartbeatTestPublish"   // A name just for humans
    ,  10000  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  1  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  TASK_CORE
  );  

  // xTaskCreatePinnedToCore(
  //   TaskTempPublish
  //   ,  "TaskTempPublish"   // A name just for humans
  //   ,  10000  // This stack size can be checked & adjusted by reading the Stack Highwater
  //   ,  NULL
  //   ,  0  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  //   ,  NULL
  //   ,  TASK_CORE
  // );  
}

void setupWifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == commands_topic) {
    // do nothing...
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt.connect(MQTT_CLIENTNAME,MQTT_USERNAME,MQTT_KEY)) {
      Serial.println("connected");
      // Subscribe
      mqtt.subscribe(commands_topic);
      mqtt.subscribe(test_topic);
      mqtt.subscribe(flow_sensor_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
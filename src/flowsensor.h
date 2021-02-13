// ***** flowsensor.h ***** 
#ifndef FLOWSENSOR_H
#define FLOWSENSOR_H

// *** EEPROM ***
// #define EEPROM_RESET // this will override eeprom data back to 0
#define EEPROM_ADDRESS 0   //Location we want the data to be put.
float debitTotalEeprom = 0.00f;

// *** FLOW SENSOR ***
#define FLOWSENSOR_PIN 2
float K = 2.6f; // Flow sensor constant, to convert debitFrequency into Liters
float debitCountTemporary = 0.00f;
extern volatile unsigned long debitFrequency = 0;

// *** FLOW SENSOR DEBUG ***
// #define DEBIT_DUMMY_DATA_ON
// #define DEBUG_MODE_ON
#define DEBIT_DUMMY_DATA_INTERVAL 1
#ifdef DEBUG_MODE_ON
  const long DEBIT_COUNT_INTERVAL = 5; // penghitungan data tiap sekian detik
  const long DEBIT_REPORT_INTERVAL = 15; // pengiriman data ke MQTT Broker tiap sekian detik
#else
  const long DEBIT_COUNT_INTERVAL = 30; // penghitungan data tiap sekian detik
  const long DEBIT_REPORT_INTERVAL = 1800; // pengiriman data ke MQTT Broker tiap sekian detik
#endif

//======================================================================//
// Headers
#include <EEPROM.h>
// #include "tasks.h"

//======================================================================//
// Prototypes
void setupFlowSensor();
void interruptFlowSensor();
void interruptFlowSensorDummy();
void printdebitCountTemporary();
void reportdebitCountTemporary();
void publishMqtt(String, String);
void resetDebitFrequency();
void updatedebitCountTemporary();

void setupEeprom();
void updateEepromData();

//======================================================================//
// Functions

// *** Flow Sensor ***
void setupFlowSensor() 
{
  // initializes flow sensor
  Serial.println("Setup: Initializing Flow Sensor...");
 
  pinMode(FLOWSENSOR_PIN, INPUT);
  digitalWrite(FLOWSENSOR_PIN, HIGH);
  attachInterrupt(0,interruptFlowSensor,RISING); // di pin 2
  sei();

  #ifdef DEBIT_DUMMY_DATA_ON
      Alarm.timerRepeat(DEBIT_DUMMY_DATA_INTERVAL, interruptFlowSensorDummy); // turn on repeater to count debit
  #endif

  #ifdef TASKS_H
    setupTasks();
  #endif
}

void interruptFlowSensor() {
  debitFrequency++;
}

void interruptFlowSensorDummy() {
  int randomAdd = random(0,10);
  debitFrequency += randomAdd;
}

void updatedebitCountTemporary() {
  debitCountTemporary = (debitFrequency/7.5)*K/60.0/DEBIT_COUNT_INTERVAL; //L/s
}

void printdebitCountTemporary() {
  updatedebitCountTemporary();

  // Prints Debit Information
  Serial.print("debitFrequency: ");
  Serial.println(debitFrequency);
  Serial.print("Debit Air: ");
  Serial.print(debitCountTemporary);
  Serial.println(" L/s");
}

void reportdebitCountTemporary() {
  updatedebitCountTemporary();

  String topic = "flow_sensor";
  String message = String(debitCountTemporary);
  publishMqtt(topic,message);

  // Update total sum in eeprom
  updateEepromData();

  // Reset DebitCount
  resetDebitFrequency();
}

void publishMqtt(String topic, String message) {
    String topicFull = "waterbox/W0001/" + topic;
    Serial.println("pub:" + topicFull + ":" + message);
    // add code to actually publish to server
}

void resetDebitFrequency() {
  Serial.println("Debit Count Reset!");
  debitFrequency = 0;
}

// *** EEPROM ***
void setupEeprom() {
  #ifdef EEPROM_RESET
    debitTotalEeprom = 0.00f;  //Variable to store in EEPROM.
    EEPROM.put(EEPROM_ADDRESS, debitTotalEeprom);
    Serial.print("Put Eeprom value: ");
    Serial.println(debitTotalEeprom);
  #endif

  EEPROM.get(EEPROM_ADDRESS, debitTotalEeprom);
  Serial.print("Get Eeprom value: ");
  Serial.println(debitTotalEeprom);
}

void updateEepromData() {
  debitTotalEeprom += debitCountTemporary;

  // put to eeprom
  EEPROM.put(EEPROM_ADDRESS, debitTotalEeprom);
  Serial.print("Put Eeprom value: ");
  Serial.println(debitTotalEeprom);
}

#endif // FLOWSENSOR_H
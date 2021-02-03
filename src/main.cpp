#include <EEPROM.h>
#include <Time.h>
#include <TimeAlarms.h>
// #include <ds3231.h>
// #include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <Wire.h>
// #include <Arduino_FreeRTOS.h>

//======================================================================//

// GLOBAL VARIABLES
// Serial
SoftwareSerial EspSerial(5, 6);

// SIM MODULE
SoftwareSerial sim(10, 9);
#ifndef CREDENTIALS_H
  char dest_phone_no[16] = "+000000000000";
#endif
String  _buffer;
String  number = dest_phone_no;

// EEPROM
// #define EEPROM_RESET_WARNING // this will override eeprom data back to 0
const int eepromAddress = 0;   //Location we want the data to be put.
float DebitTotalEeprom;

// FLOW METER SENSOR
// #define FLOWSENSOR_DUMMY_ON
#define FLOWSENSOR_PIN 2
#ifdef debug_mode
  const long DEBIT_PRINT_INTERVAL = 5; // penghitungan data tiap sekian detik
  const long DEBIT_REPORT_INTERVAL = 15; // pengiriman data ke MQTT Broker tiap sekian detik
#else
  const long DEBIT_PRINT_INTERVAL = 30; // penghitungan data tiap sekian detik
  const long DEBIT_REPORT_INTERVAL = 1800; // pengiriman data ke MQTT Broker tiap sekian detik
#endif
float K = 2.6f; // Konstanta flow sensor
float DebitCountTemporary = 0.00f;
volatile unsigned long debitFrequency = 0;

// RTC
// struct ts t;

// SD CARD
// File myFile;

// TIME
// AlarmId id;

//======================================================================//

// Headers
#include "serial.h"
#include "flowsensor.h"
// #include "rtc.h"
// #include "sdcard.h"
#include "sim.h"
// #include "tasks.h"
// #include "alarms.h"

//======================================================================//

void setup()
{
  // ******* Setup_Start ******* 
  setupSerial();
  setupEeprom();
  setupSim(); // [SALMAN] cek ini
  // setupRTC(); // abaikan aja ini
  setupFlowSensor(); // abaikan aja ini
  // setupSDCard(); // [SALMAN] cek ini
  // ******* Setup_End *******

  setTime(22,15,30,23,1,21); // set initial time, can be fixed later
  Alarm.timerRepeat(DEBIT_PRINT_INTERVAL, printDebitCountTemporary); // turn on repeater to count debit
  Alarm.timerRepeat(DEBIT_REPORT_INTERVAL, reportDebitCountTemporary); // turn on repeater to publish debit to MQTT Broker
  
  Serial.println("Setup: Initialization done.");
}

void loop() 
{
  echo();
  listenEsp();
  Alarm.delay(1); // wait one second between clock display. MUST BE TURNED ON TO USE Alarm::timerRepeater
}

//======================================================================//








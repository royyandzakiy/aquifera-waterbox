// ***** main.cpp *****
// Libraries

#include <SoftwareSerial.h>
#include <SPI.h>
#include <Time.h>
#include <TimeAlarms.h>

//======================================================================//
// Global Variables

// *** SIM ***
#ifndef CREDENTIALS_H
  char dest_phone_no[16] = "+000000000000";
#endif

//======================================================================//
// Headers

#include "serial.h" 
#include "flowsensor.h"
#include "sim.h"
// #include "alarms.h" // *** THIS FILE IS NOT YET USED ***
// #include "rtc.h" // *** THIS FILE IS NOT YET USED ***
// #include "sdcard.h" // *** THIS FILE IS NOT YET USED ***

//======================================================================//
// Main

void setup()
{
  // ******* Setup_Start *******

  setupSerial();
  setupEeprom();
  setupSim();
  setupFlowSensor();
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
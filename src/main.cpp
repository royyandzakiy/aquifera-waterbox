//Volume air akan dihitung setiap 5 detik dan dijumlahkan secara langsung
//Tiap jam akan disimpan data berupa jumlah volume air pada jam tersebut sehingga
//dapat diketahui pengeluaran terbesar pada jam-jam berapa saja

// #include <EEPROM.h>
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

// FLOW METER SENSOR
#define FLOWSENSOR_PIN 2
#define FLOWSENSOR_DUMMY_ON
const long DEBIT_OOUNT_INTERVAL = 5; // penghitungan data tiap sekian detik
const long DEBIT_REPORT_INTERVAL = 15; // pengiriman data ke MQTT Broker tiap sekian detik
float K = 2.6; // Konstanta flow sensor
float DebitAir = 0.0;
volatile unsigned long frekuensi_aliran = 0;

// RTC
// struct ts t;

// SD CARD
// File myFile;

// TIME
// AlarmId id;

//======================================================================//

// Prototypes
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
  setupSim(); // [SALMAN] cek ini
  // setupRTC(); // abaikan aja ini
  setupFlowSensor(); // abaikan aja ini
  // setupSDCard(); // [SALMAN] cek ini
  // ******* Setup_End *******

  setTime(22,15,30,23,1,21); // set initial time, can be fixed later
  Alarm.timerRepeat(DEBIT_OOUNT_INTERVAL, updateDebitCount); // turn on repeater to count debit
  Alarm.timerRepeat(DEBIT_REPORT_INTERVAL, reportDebitCount); // turn on repeater to publish debit to MQTT Broker
  
  Serial.println("Setup: Initialization done.");
}

void loop() 
{
  echo();
  listenEsp();
  Alarm.delay(1); // wait one second between clock display. MUST BE TURNED ON TO USE Alarm::timerRepeater
}

//======================================================================//








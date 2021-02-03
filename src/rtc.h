// ***** rtc.h ***** 
// *** THIS FILE IS NOT YET USED ***

#ifndef RTC_H
#define RTC_H

#include <ds3231.h>
#include <Wire.h>

// RTC
struct ts t;

//======================================================================//
// Prototypes

void setupRTC();
void testRTC();

//======================================================================//
// Functions

void setupRTC() {
 // initializes RTC
 Serial.println("Setup: Initializing RTC...");

 Wire.begin();
 DS3231_init(DS3231_CONTROL_INTCN);

 testRTC();
}

void testRTC() {
 // set time
 t.hour=12; 
 t.min=30;
 t.sec=0;
 t.mday=25;
 t.mon=12;
 t.year=2020;
 
 DS3231_set(t);

 // get time
 DS3231_get(&t);
 Serial.print("Date : ");
 Serial.print(t.mday);
 Serial.print("/");
 Serial.print(t.mon);
 Serial.print("/");
 Serial.print(t.year); 
 Serial.print("\t Hour : ");
 Serial.print(t.hour);
 Serial.print(":");
 Serial.print(t.min);
 Serial.print(".");
 Serial.print(t.sec);
 Serial.println();  
}

#endif // RTC_H
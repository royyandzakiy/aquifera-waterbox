// ***** alarms.h ***** 

#ifndef ALARMS_H
#define ALARMS_H

#include <Time.h>
#include <TimeAlarms.h>

// AlarmId id;

//======================================================================//
// Prototypes

void testAlarms();
void digitalClockDisplay();
void printDigits(int);

//======================================================================//
// Functions

void testAlarms() {
    void digitalClockDisplay();   
}

void digitalClockDisplay() {
 // digital clock display of the time
 Serial.print(hour());
 printDigits(minute());
 printDigits(second());
 Serial.println();
}

void printDigits(int digits) {
 Serial.print(":");
 if (digits < 10)
   Serial.print('0');
 Serial.print(digits);
}

#endif // ALARMS_H
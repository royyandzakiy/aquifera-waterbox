// ***** sim.h *****

#ifndef SIM_H
#define SIM_H

// SIM MODULE
SoftwareSerial sim(10, 9);
String  _buffer;
String  number = dest_phone_no;

//======================================================================//
// Prototypes
void setupSim();
void testSim();

//======================================================================//
// Functions

// Setup SIM
void setupSim() {
 Serial.println("Setup: Initializing SIM...");
 // delay(7000); //delay  detik untuk memastikan modul sim sudah siap 
 _buffer.reserve(50);
 sim.begin(9600);

 testSim();
}

void testSim() {
 // send GPRS here
 // ...still nothing
}

#endif // SIM_H
// SIM
void setupSim();
void testSim();

// ***** SIM ***** 
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
void testAlarms();
void digitalClockDisplay();
void printDigits(int);

// ***** TIME ***** 
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
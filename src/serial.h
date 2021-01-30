// Serial
void setupSerial();
void echo();
void listenEsp();
void sendEsp(String);
void processListenEsp(String);

// ***** SERIAL ***** 
void echo() {
  if(Serial.available()) {
    String str = Serial.readStringUntil('\n');
    Serial.println(str);
    sendEsp(str);
    delay(10);
  }
}

void listenEsp() {
  if(EspSerial.available()) {
    String str = EspSerial.readStringUntil('\n');
    Serial.println(str);

    processListenEsp(str);
    delay(10);
  }
}

void processListenEsp(String str) {
  if (str=="set_time") {
    // set time of internal Alarm.time
    Serial.println("set_time command engegaed");
  } else if (str=="get_time") {
    // send current internal Alarm.time
    Serial.println("get_time command engegaed");
  } else if (str=="set_time_rtc") {
    // set current internal time from rtc
    Serial.println("set_time_rtc command engegaed");
  } else if (str=="set_time_rtc") {
    // send current internal time from rtc
    Serial.println("get_time_rtc command engegaed");
  } else if (str=="get_info_waterbox") {
    // send info of waterbox ID
    Serial.println("get_info_waterbox command engegaed");
  } else if (str=="get_debit_count") {
    // send current debit count
    Serial.println("get_debit_count command engegaed");
  } else if (str=="get_debit_data") {
    // send debit data in sd card
    Serial.println("get_debit_data command engegaed");
  }
}

void sendEsp(String str) {
  EspSerial.println(str);
}

void setupSerial() {
  Serial.begin(9600);
  EspSerial.begin(9600);

  while (!Serial);
  Serial.println("Setup: Initialize Serial...");
}
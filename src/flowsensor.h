// Flow Sensor
void setupFlowSensor();
void interruptFlowSensor();
void interruptFlowSensorDummy();
void printDebitCountTemporary();
void reportDebitCountTemporary();
void publishMqtt(String, String);
void resetDebitFrequency();
void updateDebitCountTemporary();
void updateEepromData();

// ***** FLOW SENSOR ***** 
#define FLOWSENSOR_DUMMY_INTERVAL 1

void setupFlowSensor() 
{
  // initializes flow sensor
  Serial.println("Setup: Initializing Flow Sensor...");
 
  pinMode(FLOWSENSOR_PIN, INPUT);
  digitalWrite(FLOWSENSOR_PIN, HIGH);
  attachInterrupt(0,interruptFlowSensor,RISING); // di pin 2
  sei();

  #ifdef FLOWSENSOR_DUMMY_ON
      Alarm.timerRepeat(FLOWSENSOR_DUMMY_INTERVAL, interruptFlowSensorDummy); // turn on repeater to count debit
  #endif

  //setupTasks();
}

void setupEeprom() {
  #ifdef EEPROM_RESET_WARNING
    DebitTotalEeprom = 0.00f;  //Variable to store in EEPROM.
    EEPROM.put(eepromAddress, DebitTotalEeprom);
    Serial.print("Put Eeprom value: ");
    Serial.println(DebitTotalEeprom);
  #else
    EEPROM.get(eepromAddress, DebitTotalEeprom);
    Serial.print("Get Eeprom value: ");
    Serial.println(DebitTotalEeprom);
  #endif
}

void interruptFlowSensor()
{
  debitFrequency++;
}

void interruptFlowSensorDummy() {
  int randomAdd = random(0,10);
  debitFrequency += randomAdd;
}

void updateDebitCountTemporary() {
  DebitCountTemporary = (debitFrequency/7.5)*K/60.0/DEBIT_PRINT_INTERVAL; //L/s
}

void printDebitCountTemporary()
{
  updateDebitCountTemporary();

  // Prints Debit Information
  Serial.print("debitFrequency: ");
  Serial.println(debitFrequency);
  Serial.print("Debit Air: ");
  Serial.print(DebitCountTemporary);
  Serial.println(" L/s");
}

void reportDebitCountTemporary() {
  updateDebitCountTemporary();

  String topic = "flow_sensor";
  String message = String(DebitCountTemporary);
  publishMqtt(topic,message);

  // Update total sum in eeprom
  updateEepromData();

  // Reset DebitCount
  resetDebitFrequency();
}

void publishMqtt(String topic, String message) {
    String topicFull = "waterbox/W0001/" + topic;
    Serial.println("pub:" + topicFull + ":" + message);
    EspSerial.println("pub:" + topicFull + ":" + message);
}

void resetDebitFrequency() {
  Serial.println("Debit Count Reset!");
  debitFrequency = 0;
}

// EEPROM
void updateEepromData() {
  DebitTotalEeprom += DebitCountTemporary;

  // put to eeprom
  EEPROM.put(eepromAddress, DebitTotalEeprom);
  Serial.print("Put Eeprom value: ");
  Serial.println(DebitTotalEeprom);
}
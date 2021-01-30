// Flow Sensor
void setupFlowSensor();
void interruptFlowSensor();
void interruptFlowSensorDummy();
void updateDebitCount();
void reportDebitCount();
void publishMqtt(String, String);
void resetDebitCount();
void printDebitCount();

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

void interruptFlowSensor()
{
  frekuensi_aliran++;
}

void interruptFlowSensorDummy() {
  int randomAdd = random(0,10);
  frekuensi_aliran += randomAdd;
}

void updateDebitCount()
{
    DebitAir = (frekuensi_aliran/7.5)*K/60.0/DEBIT_OOUNT_INTERVAL; //L/s

    // Print Debit Information
    printDebitCount();
}

void printDebitCount() {
  Serial.print("frekuensi_aliran: ");
  Serial.println(frekuensi_aliran);
  Serial.print("Debit Air: ");
  Serial.print(DebitAir);
  Serial.println(" L/s");
}

void reportDebitCount() {
  String topic = "flow_sensor";
  String message = String(DebitAir);
  publishMqtt(topic,message);
  resetDebitCount();
}

void publishMqtt(String topic, String message) {
    String topicFull = "waterbox/W0001/" + topic;
    Serial.println("pub:" + topicFull + ":" + message);
    EspSerial.println("pub:" + topicFull + ":" + message);
}

void resetDebitCount() {
  Serial.println("Debit Count Reset!");
  frekuensi_aliran = 0;
}

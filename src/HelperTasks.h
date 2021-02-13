void extractCommandArduino(String);
void commandArduino(String, String, String);
void TaskHeartbeatTestPublish(void *pvParameters);

void TaskHeartbeatTestPublish(void *pvParameters) {
  // test publish
  (void) pvParameters;
  int count = 0;
  
  for(;;) {
    if (mqtt.connected()) {      
      count++;
      // Publish
      // Serial.print(F("Test Publish Success! Published: "));
      String str = "pub:waterbox/W0001/test:" + String(count);
      Serial.println(str);
      extractCommandArduino(str);
    } else {
      // Serial.println("Publish Failed! Not Connected to MQTT");
    }
    vTaskDelay(5000);
  }
}

void extractCommandArduino(String str) {
  // extract the command string sent by arduino through uart
  String command, topicFull, topic, message;
  int separatorPos = str.indexOf(":");
  if (separatorPos > -1) {
    command = str.substring(0,separatorPos);
    topicFull = str.substring(separatorPos+1, str.indexOf(":",separatorPos+1));
    topic = topicFull.substring(topicFull.indexOf("/",10)+1);
    separatorPos = str.indexOf(":",separatorPos+1);
    message = str.substring(separatorPos+1);

    commandArduino(command, topic, message);
    
    // Serial.println(command);
    // Serial.println(topic);
    // Serial.println(message);
  }
}

void commandArduino(String command, String topic, String _message) {
  /**
   * *********** FUNCTION LIST *********** 
   * publish
   * set_time
   * get_time
   * set_time_rtc
   * get_time_rtc
   * get_info_waterbox
   * get_debit_count
   * get_debit_data
  */
 char* message;
 _message.toCharArray(message, _message.length());
  if (command == "pub") {
    if (topic == "flow_sensor") {
      mqtt.publish(flow_sensor_topic, message);
    } else if (topic == "temp_sensor") {
      mqtt.publish(temp_sensor_topic, message);
    } else if (topic == "test") {
      mqtt.publish(test_topic, message);
    } else {
      Serial.println("topic error");
    } 
  } else if ("set_time") {
      Serial.println(command);
  } else if ("get_time") {
      Serial.println(command);
  } else if ("set_time_rtc") {
      Serial.println(command);
  } else if ("get_time_rtc") {
      Serial.println(command);
  } else if ("get_info_waterbox") {
      Serial.println(command);
  } else if ("get_debit_count") {
      Serial.println(command);
  } else if ("get_debit_data") {
      Serial.println(command);
  } else {
      Serial.println("nothing todo...");
  } 
}
const int BUFFER_SIZE = 1200;
const char* send_cmd = "SendIR";
const char* lean_cmd = "LeanIR";
const char* get_cmd = "Get_infor";


void callback(char* topic, byte* payload, unsigned int length) {
  //Serial.print("Message arrived [");
  //Serial.print(topic);
  //Serial.print("] ");
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  //Serial.println(message);
    if (!processJson(message)) {
    return;
  }
}
boolean reconnect() {
  //Serial.println("Reconnecting");
  if (statusmang==1){
  if (clientmqtt.connect("arduinoClient",mqtt_user, mqtt_pwd)) {
   // if (clientmqtt.connect("arduinoClient")) {
    clientmqtt.publish(mqtt_topic,"Reconnect");
    clientmqtt.subscribe(mqtt_topic);
  }
  
  }
  return clientmqtt.connected();
}

bool processJson(char* message) {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);
  if (!root.success()) {
    //Serial.println("parseObject() failed");
  return false;
  } 
  if (root.containsKey("command")) {
  }
  return true;
}

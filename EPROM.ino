void printWiFiConf(void) {
 // Serial.println(WiFiConf.sta_ssid);
}

bool loadWiFiConf() {
  if (EEPROM.read(WIFI_CONF_START + 0) == wifi_conf_format[0] &&
      EEPROM.read(WIFI_CONF_START + 1) == wifi_conf_format[1] &&
      EEPROM.read(WIFI_CONF_START + 2) == wifi_conf_format[2] &&
      EEPROM.read(WIFI_CONF_START + 3) == wifi_conf_format[3])
  {
    for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
      *((char*)&WiFiConf + t) = EEPROM.read(WIFI_CONF_START + t);
      //Serial.println(WIFI_CONF_START + t);
    }
    printWiFiConf();
    return true;
  } else {
    return false;
  }
}
////////////////////////////////////
//Lưu thông số vào eeprom /////////
//////////////////////////////////
void saveWiFiConf(void) {
  for (unsigned int t = 0; t < sizeof(WiFiConf); t++) {
    EEPROM.write(WIFI_CONF_START + t, *((char*)&WiFiConf + t));
  }
  EEPROM.commit();
  printWiFiConf();
}
void setDefaultModuleId(char* dst) {
  uint8_t macAddr[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(macAddr);
  sprintf(dst, "%s%02x%02x", NAME_PREF, macAddr[WL_MAC_ADDR_LENGTH - 2], macAddr[WL_MAC_ADDR_LENGTH - 1]);
}
void resetModuleId(void) {
  uint8_t macAddr[WL_MAC_ADDR_LENGTH];
  WiFi.macAddress(macAddr);
  setDefaultModuleId(WiFiConf.module_id);
}

void EEPROM_writeDouble(int ee, float value)
{
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
       EEPROM.write(ee++, *p++);
   
}

float EEPROM_readDouble(int ee)
{
   float value = 0.0;
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
       *p++ = EEPROM.read(ee++);
   return value;
}

void read_sensor_eeprom(){
  int diachi=500;
  Serial.println("Begin EEPROM");
        for (unsigned int t = 0; t < 30; t++) {
      *((float*)&SensorStruct + t) = EEPROM_readDouble(diachi + t*4);
      Serial.println(*((float*)&SensorStruct + t));
    }
    Serial.println("END EEPROM");
}
void write_sensor_eeprom(){
  int diachi=500; 
  for (unsigned int t = 0; t < 30; t++) {
    float tt=*((float*)&SensorStruct + t);
    EEPROM_writeDouble(diachi + t*4, tt);
    Serial.println(tt);
  }
  EEPROM.commit();
//  printWiFiConf();
  
}

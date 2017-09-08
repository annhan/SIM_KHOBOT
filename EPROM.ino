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


void EEPROMWriteInt(int p_address,unsigned int p_value)
     {
     byte lowByte = ((p_value >> 0) & 0xFF);
     byte highByte = ((p_value >> 8) & 0xFF);

     EEPROM.write(p_address, lowByte);
     EEPROM.write(p_address + 1, highByte);
     }

//This function will read a 2 byte integer from the eeprom at the specified address and address + 1
unsigned int EEPROMReadInt(int p_address)
     {
     byte lowByte = EEPROM.read(p_address);
     byte highByte = EEPROM.read(p_address + 1);
     unsigned int trave= ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
   //  Serial.println(trave);
     return trave;
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
  int diachi=600;
 // Serial.println("Begin EEPROM");
        for (int t = 0; t < 15; t++) {
      *((unsigned int*)&SensorStruct + ((t*8)+0)) = EEPROMReadInt(diachi + (t*32));
      unsigned int hhhh=*((unsigned int*)&SensorStruct + ((t*8)+0));
      Serial.print("SO TAG : ");
      Serial.println(hhhh);

      *((unsigned int*)&SensorStruct + ((t*8)+1)) = EEPROMReadInt(diachi + ((t*32)+4));
     // Serial.println(*((unsigned int*)&SensorStruct + ((t*6)+1)));

      *((float*)&SensorStruct + ((t*8)+2)) = EEPROM_readDouble(diachi + ((t*32)+8));
      //Serial.println(*((float*)&SensorStruct + ((t*6)+2)));

      *((float*)&SensorStruct + ((t*8)+3)) = EEPROM_readDouble(diachi + ((t*32)+12));
     // Serial.println(*((float*)&SensorStruct + ((t*6)+3)));

      *((float*)&SensorStruct + ((t*8)+4)) = EEPROM_readDouble(diachi + ((t*32)+16));
     // Serial.println(*((float*)&SensorStruct + ((t*6)+4)));

      *((float*)&SensorStruct + ((t*8)+5)) = EEPROM_readDouble(diachi + ((t*32)+20));
     // Serial.println(*((float*)&SensorStruct + ((t*6)+5)));
      
    }
   // Serial.println("END EEPROM");
}
void write_sensor_eeprom(){
  int diachi=600; 

  for (int t = 0; t < 15; t++) {
      unsigned int tt=*((unsigned int*)&SensorStruct + ((t*8)+0));
      EEPROMWriteInt(diachi + (t*32), tt);
     // Serial.print("tab: ");
      //Serial.println(tt);
      
      unsigned int tt1=*((unsigned int*)&SensorStruct + ((t*8)+1));
      EEPROMWriteInt(diachi + ((t*32)+4), tt1);
     // Serial.print("zone: ");
     // Serial.println(tt1);

      float tt2=*((float*)&SensorStruct + ((t*8)+2));
      EEPROM_writeDouble(diachi + ((t*32)+8), tt2);
     // Serial.println(tt2);

      float tt3=*((float*)&SensorStruct + ((t*8)+3));
      EEPROM_writeDouble(diachi + ((t*32)+12), tt3);
     // Serial.println(tt3);

      float tt4=*((float*)&SensorStruct + ((t*8)+4));
      EEPROM_writeDouble(diachi + ((t*32)+16), tt4);
     // Serial.println(tt4);

      float tt5=*((float*)&SensorStruct + ((t*8)+5));
      EEPROM_writeDouble(diachi + ((t*32)+20), tt5);
     // Serial.println(tt5);
    }
  EEPROM.commit();
}

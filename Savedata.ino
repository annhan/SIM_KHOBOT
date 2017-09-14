//*******************
// Hafm tach IP.Gateway
//*******************
void parseBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
    for (i = 0; i < maxBytes; i++) {
        bytes[i] = strtoul(str, NULL, base);  // Convert byte
        str = strchr(str, sep);               // Find next separator
        if (str == NULL || *str == '\0') {
            break;                            // No more separators, exit
        }
        str++;                                // Point to next character after separator
    }
}
void parseBytes1(const char* str, char sep, int address, int maxBytes, int base) {
  for (int i = 0; i < maxBytes; i++) {
    if (address == 1) {ip10[i] = strtoul(str, NULL, base);
    //Serial.println(ip10[i]);
    }  // Convert byte
    else if (address == 2) {gateway10[i] = strtoul(str, NULL, base);
    //Serial.println(gateway10[i]);
    }  // Convert byte
    else if (address == 3) {subnet10[i] = strtoul(str, NULL, base);
    //Serial.println(subnet10[i]);
    }  // Convert byte
    
    str = strchr(str, sep);               // Find next separator
    
    if (str == NULL || *str == '\0') {
      break;                            // No more separators, exit
    }
    str++;                                // Point to next character after separator
  }
}
void connect_wifi(){
  
  WiFi.setAutoReconnect(true);
  WiFi.begin(WiFiConf.sta_ssid, WiFiConf.sta_pwd);
  parseBytes1(WiFiConf.sta_ip, '.', 1, 4, 10);
  parseBytes1(WiFiConf.sta_gateway, '.', 2, 4, 10);
  parseBytes1(WiFiConf.sta_subnet, '.', 3, 4, 10);
  WiFi.config(ip10,gateway10,subnet10,DNS);
}
long tacksotustring(String kitu){
 // Serial.println(kitu);
  long trave=100000;
  char a[12];
  int len=kitu.length();
  char s[len];
  kitu.toCharArray(s, sizeof(s));
  int j,dai; 
    len=strlen(s);
    byte thoat=0;
    j=0;
    if (len>7){
    for(int i=8;i<len;i++)
    {
        if(s[i]>='0' && s[i]<='9')
        {
           if (thoat == 0 || thoat == 1){
            a[j++]=s[i];
            thoat=1;
           }
        }
        else if (thoat == 1){thoat=2;da_kttk= true;};
    }
    a[j] = 0;
    trave=atol(a);
    }
    Serial.println(trave);
    return  trave;
    
    
}


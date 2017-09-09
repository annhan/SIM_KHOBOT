///////////////////////////////////////////////////////////////////////
///Ham Chuyen doi user:password qua Base64 cho giao tiep qua mang///////
////////////////////////////////////////////////////////////////////////
const char PROGMEM b64_alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

/* 'Private' declarations */
void a3_to_a4(unsigned char * a4, unsigned char * a3);
void a4_to_a3(unsigned char * a3, unsigned char * a4);
unsigned char b64_lookup(char c);

int base64_encode(char *output, char *input, int inputLen) {
  int j = 0;
  i = 0;
  int encLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];

  while(inputLen--) {
    a3[i++] = *(input++);
    if(i == 3) {
      a3_to_a4(a4, a3);

      for(i = 0; i < 4; i++) {
        output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);
      }

      i = 0;
    }
  }

  if(i) {
    for(j = i; j < 3; j++) {
      a3[j] = '\0';
    }

    a3_to_a4(a4, a3);

    for(j = 0; j < i + 1; j++) {
      output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);
    }

    while((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;
}

int base64_decode(char * output, char * input, int inputLen) {
  int j = 0;
  i = 0;
  int decLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];


  while (inputLen--) {
    if(*input == '=') {
      break;
    }

    a4[i++] = *(input++);
    if (i == 4) {
      for (i = 0; i <4; i++) {
        a4[i] = b64_lookup(a4[i]);
      }

      a4_to_a3(a3,a4);

      for (i = 0; i < 3; i++) {
        output[decLen++] = a3[i];
      }
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 4; j++) {
      a4[j] = '\0';
    }

    for (j = 0; j <4; j++) {
      a4[j] = b64_lookup(a4[j]);
    }

    a4_to_a3(a3,a4);

    for (j = 0; j < i - 1; j++) {
      output[decLen++] = a3[j];
    }
  }
  output[decLen] = '\0';
  return decLen;
}

int base64_enc_len(int plainLen) {
  int n = plainLen;
  return (n + 2 - ((n + 2) % 3)) / 3 * 4;
}

int base64_dec_len(char * input, int inputLen) {
  i = 0;
  int numEq = 0;
  for(i = inputLen - 1; input[i] == '='; i--) {
    numEq++;
  }
  return ((6 * inputLen) / 8) - numEq;
}

void a3_to_a4(unsigned char * a4, unsigned char * a3) {
  a4[0] = (a3[0] & 0xfc) >> 2;
  a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
  a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
  a4[3] = (a3[2] & 0x3f);
}

 void a4_to_a3(unsigned char * a3, unsigned char * a4) {
  a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
  a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
  a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

unsigned char b64_lookup(char c) {
  if(c >='A' && c <='Z') return c - 'A';
  if(c >='a' && c <='z') return c - 71;
  if(c >='0' && c <='9') return c + 4;
  if(c == '+') return 62;
  if(c == '/') return 63;
  return -1;
}
/////////////////////////////////////////////////////////////////////////

////////////////////////////////////
//do wifi /////////
//////////////////////////////////
void scanWiFi(void) {
  int founds = WiFi.scanNetworks();
  network_html = F("<ol>");
  for (i = 0; i < founds; ++i)
  {
    network_html += F("<li>");
    network_html += WiFi.SSID(i);
    network_html += F(" (");
    network_html += WiFi.RSSI(i);
    network_html += F(")");
    network_html += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    network_html += F("</li>");
  }
  network_html += F("</ol>");
}
////////////////////////////////////
//chờ kết nối mạng /////////
//////////////////////////////////
int waitConnected(void) {
  int wait = 0;
  while ( wait < 20 ) {
    if (WiFi.status() == WL_CONNECTED) {
      return (1);
    }
    //yield();
    delay(700);
    wait++;
  }
  return (0);
}

void printIP(void) {
}



void setupWiFiConf(void) {
  server.on("/DataRx/", HTTP_POST, []() {
   // Serial.println("Có data server 1");
   // String message = "Body received 1:\n";
    str_html_khobot = server.arg("rrs_data");
    html_khobot=true;
    server.send(200, F("text/html"), "OK");
  });
 server.on("/get_infor_sensor", []() {
    String trave="{";
    for (int i=0;i<15;i++){
      if (i<14){
      trave +="\"Sensor" + String(i)+"\":{\"name\":\"" + String(*((unsigned int*)&SensorStruct + (i*8))) + "\",\"value\":[" + String(*((float*)&SensorStruct + ((i*8)+2))) + "," + String(*((float*)&SensorStruct + ((i*8)+3))) + ","+ String(*((float*)&SensorStruct + ((i*8)+4))) + "," + String(*((float*)&SensorStruct + ((i*8)+5))) + ","+ String(*((float*)&SensorStruct + ((i*8)+6))) + "," + String(*((float*)&SensorStruct + ((i*8)+7))) + "]},";
      }
      else{
      trave +="\"Sensor" + String(i)+"\":{\"name\":\"" + String(*((unsigned int*)&SensorStruct + (i*8))) + "\",\"value\":[" + String(*((float*)&SensorStruct + ((i*8)+2))) + "," + String(*((float*)&SensorStruct + ((i*8)+3))) + ","+ String(*((float*)&SensorStruct + ((i*8)+4))) + "," + String(*((float*)&SensorStruct + ((i*8)+5))) + ","+ String(*((float*)&SensorStruct + ((i*8)+6))) + "," + String(*((float*)&SensorStruct + ((i*8)+7))) + "]}";
      }
    }
    trave += "}";
    Serial.println(trave);
    server.send(200, F("text/html"), trave);
 });
 server.on("/set_sensor_conf", []() {
  int vitri=1000;
  String tab;
  String zone;
  String value1;
  String value2;
  String value3;
  String value4;
    if (server.hasArg("tab0")){
      vitri=0;
      tab=server.arg("tab0");
      zone=server.arg("zone0");
      value1=server.arg("t_down0");
      value2=server.arg("t_up0");
      value3=server.arg("h_down0");
      value4=server.arg("h_up0");
    }
    else if (server.hasArg("tab1")){
      vitri=8;
      tab=server.arg("tab1");
      zone=server.arg("zone1");
      value1=server.arg("t_down1");
      value2=server.arg("t_up1");
      value3=server.arg("h_down1");
      value4=server.arg("h_up1");
    }
    else if (server.hasArg("tab2")){
      vitri=16;
      tab=server.arg("tab2");
      zone=server.arg("zone2");
      value1=server.arg("t_down2");
      value2=server.arg("t_up2");
      value3=server.arg("h_down2");
      value4=server.arg("h_up2");
    }
        else if (server.hasArg("tab3")){
      vitri=24;
      tab=server.arg("tab3");
      zone=server.arg("zone3");
      value1=server.arg("t_down3");
      value2=server.arg("t_up3");
      value3=server.arg("h_down3");
      value4=server.arg("h_up3");
    }
        else if (server.hasArg("tab4")){
      vitri=32;
            tab=server.arg("tab4");
      zone=server.arg("zone4");
      value1=server.arg("t_down4");
      value2=server.arg("t_up4");
      value3=server.arg("h_down4");
      value4=server.arg("h_up4");
    }
        else if (server.hasArg("tab5")){
      vitri=40;
            tab=server.arg("tab5");
      zone=server.arg("zone5");
      value1=server.arg("t_down5");
      value2=server.arg("t_up5");
      value3=server.arg("h_down5");
      value4=server.arg("h_up5");
    }
        else if (server.hasArg("tab6")){
      vitri=48;
            tab=server.arg("tab6");
      zone=server.arg("zone6");
      value1=server.arg("t_down6");
      value2=server.arg("t_up6");
      value3=server.arg("h_down6");
      value4=server.arg("h_up6");
    }
        else if (server.hasArg("tab7")){
      vitri=56;
            tab=server.arg("tab7");
      zone=server.arg("zone7");
      value1=server.arg("t_down7");
      value2=server.arg("t_up7");
      value3=server.arg("h_down7");
      value4=server.arg("h_up7");
    }
        else if (server.hasArg("tab8")){
      vitri=64;
            tab=server.arg("tab8");
      zone=server.arg("zone8");
      value1=server.arg("t_down8");
      value2=server.arg("t_up8");
      value3=server.arg("h_down8");
      value4=server.arg("h_up8");
    }
        else if (server.hasArg("tab9")){
      vitri=72;
            tab=server.arg("tab9");
      zone=server.arg("zone9");
      value1=server.arg("t_down9");
      value2=server.arg("t_up9");
      value3=server.arg("h_down9");
      value4=server.arg("h_up9");
    }
        else if (server.hasArg("tab10")){
      vitri=80;
            tab=server.arg("tab10");
      zone=server.arg("zone10");
      value1=server.arg("t_down10");
      value2=server.arg("t_up10");
      value3=server.arg("h_down10");
      value4=server.arg("h_up10");
    }
        else if (server.hasArg("tab11")){
      vitri=88;
            tab=server.arg("tab11");
      zone=server.arg("zone11");
      value1=server.arg("t_down11");
      value2=server.arg("t_up11");
      value3=server.arg("h_down11");
      value4=server.arg("h_up11");
    }
        else if (server.hasArg("tab12")){
      vitri=96;
            tab=server.arg("tab12");
      zone=server.arg("zone12");
      value1=server.arg("t_down12");
      value2=server.arg("t_up12");
      value3=server.arg("h_down12");
      value4=server.arg("h_up12");
    }
        else if (server.hasArg("tab13")){
      vitri=104;
            tab=server.arg("tab13");
      zone=server.arg("zone13");
      value1=server.arg("t_down13");
      value2=server.arg("t_up13");
      value3=server.arg("h_down13");
      value4=server.arg("h_up13");
    }
        else if (server.hasArg("tab14")){
      vitri=112;
            tab=server.arg("tab14");
      zone=server.arg("zone14");
      value1=server.arg("t_down14");
      value2=server.arg("t_up14");
      value3=server.arg("h_down14");
      value4=server.arg("h_up14");
    }
    if (vitri!=100){
      *((unsigned int*)&SensorStruct + vitri) = tab.toInt();
      *((unsigned int*)&SensorStruct + vitri+1) = zone.toInt();
      *((float*)&SensorStruct + vitri+2) = value1.toFloat();
      *((float*)&SensorStruct + vitri+3) = value2.toFloat();
      *((float*)&SensorStruct + vitri+4) = value3.toFloat();
      *((float*)&SensorStruct + vitri+5) = value4.toFloat();
      write_sensor_eeprom(); 
    }
    server.send(200, F("text/html"), "OK");
  });
  server.on("/getHC", []() {
    
    String content =  F("<!DOCTYPE HTML>\r\n<html><head><style>body {background-color:lightgrey}h1 {color:green}p {color:black}</style><link rel=\"shortcut icon\" href=\"data:image/x-icon;base64,AAABAAEAEBAAAAEACABoBQAAFgAAACgAAAAQAAAAIAAAAAEACAAAAAAAAAEAAAAAAAAAAAAAAAEAAAAAAADLy8sAHYsbACCNGADK28UADIEHAHOubwAShQoA//7/AObz6ADn8+gA6fPoAOzz6ADGxsYAXa5eAL7gugAghxAAtbyyABFmCAD09vEAN5ctADuPMAA4ly0AxuG9AKvXqQA2lzYAJ4cZADuXNgAikCIAKIwoAHS1cwDs++YAAHgAAKrKpwAAewAAj8WQACCHEQDa6tgAtbyzAMTcwQA4jzEAOpEuAFCkSwAYZgkAa7JiAIK9ggBAmDoAQJY9AA6FDADQ5NAApMalAC2PKQCozagA9PXwAPT48AAvbScAF2YKAGywZgBtsGYA4+3iAMviywA/mDsARpgyAM/jzgBkkGEA0eXLAC6KJAA2dDMAotCgAEWbRAA5iiQAv9+6AKmr3AAQZQgA8vXxAPn48QA+mDMAsdayAJ7DngChxKEANnQ0ACiRKABlkGIAq7iqAAB3AAAAegAAjsSQAHm5eQDKycoAw9vBAGGxYgB/mqMAhJesAPr6+AC21qoA/v/+AJqamgBygMIASpxAAMjIyADv8PYAYLBgANrr1gCUx5EAZ61gAGitYADQy9EAKGpOACOPJAD7+/wAJI8kAPv9+QDo8OUA/v//AI24iQD///8AWaxVAC+MJABerFUAXaleAF6pXgB5tHUA2uvXAPX16wDPys8AxOLGANHL0gAnhB8Agr1+AJqh1AAOfAgAzOHMAIe+gQCLvoEA6O7pAHK0bQCrt6oAMI4iAC6QKAAtlCsAS5s/AF+rXAB4t3YAwdrBAMrIygDv+e8A3O3eAM/K0ADf4uoAdJdxAHWXcQAniBoAbqtuAP3+9QAMhgwAoc6fAHO0bgAvjiMAFYYMAI28jgBGn0MAyMfIAGOrXQB4uHoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAyPe4c/NhEqT5QlaaAAX3JyXiITc5oWDmYpS11ycgByXIw8ZXJycnJycnxFg3JiclZESXJycnJycnJyOkEXfSScCXIvG1BtbW1rnYVAPRB4m3JyGgRxMU1OnoFycimVLlhycllTjXJycpAccnJmQpYLcnJkHx8zcnIeAnJyRjcPenJyDVNUBnJjgGBHchZIFDAHIAEoHWxyk1tqWnJDNjmDcm+KdCxycm4FK5hydVE7MhJyl34hGAeff3JyeRVScndoB3JyjpkfTHJyNC1VknIKYWcScnKRA3JyCIuhcFdycgl2iYQ+SjUmhoiiXHIMcnJycoI4JyMZLngkcnJyDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=\"/><meta charset='UTF-8'><title>");
    content += F("Wait");
    getHC();
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor1_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=0;
    content1 += FPSTR(fieldset);
    content1 += FPSTR(legend_html);
    content1 += ">Sensor"+String(bien_tang_sensor);
    content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8))));
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });  
  
server.on("/sensor2_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
     int bien_tang_sensor=1;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5)));
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor3_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=2;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor4_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=3;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor5_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=4;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor6_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=5;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor7_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=6;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor8_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=7;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor9_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=8;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor10_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=9;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor11_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=10;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor12_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=11;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor13_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=12;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor14_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=13;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/sensor15_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += F("</p><form method='get' action='set_sensor_conf'>");
    int bien_tang_sensor=14;
      content1 += FPSTR(fieldset);
      content1 += FPSTR(legend_html);
      content1 += ">Sensor"+String(bien_tang_sensor);
      content1 += FPSTR(_legend_html);
      
    content1 += F("<li><label for='tabid");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">TAB ID ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='tab");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='tab"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='zone");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">Zone ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='zone");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='zone"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((unsigned int*)&SensorStruct + ((bien_tang_sensor*8)+1))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Status ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+6))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_status");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">H STATUS");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_status");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_status"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+7))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+2))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='t_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">T Up ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='t_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='t_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+3))) ;
    content1 +=  F(" ><br /><br />");
        content1 += F("<li><label for='h_down");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM DOWN ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_down");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_down"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+4))) ;
    content1 +=  F(" ><br /><br />");
    content1 += F("<li><label for='h_up");
    content1 +=String(bien_tang_sensor); 
    content1 +=F("' class=\"req\">HUM UP ");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F(" : </label><input name='h_up");
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' class=\"txt\" id='h_up"); 
    content1 +=String(bien_tang_sensor) ;
    content1 +=F("' maxlength=10 value=");
    content1 += String(*((float*)&SensorStruct + ((bien_tang_sensor*8)+5))) ;
    content1 +=  F(" ><br /><br />");
    content1 += FPSTR(_fieldset);
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    
    content += F("<h1>Cài Đặt Wifi</h1>");
      content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    content += F(" </p>");

    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  
  server.on("/wifi_conf", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    String   content1 = F("<p>Wifi Connected: ");
    content1 += WiFiConf.sta_ssid;
    content1 += F("</br>IP Address: ");
    content1 += ipStr;
    content1 += F(" ( ");
    content1 += WiFiConf.module_id;
    content1 += F(".local");
    content1 += F(" )</p>");
    content1 += F("<p>");
     content1 += F("</p><form method='get' action='set_wifi_conf'>");
    content1 += F("<li><label for='ssid' class=\"req\">SSID : </label><input name='ssid' class=\"txt\" id='ssid' maxlength=32 value=") ;
    content1 += String(WiFiConf.sta_ssid) ;
    content1 +=  F(" ><br /><br />");

    content1 += F("<li><label for='pwd' class=\"req\">PASS : </label> <input type='password' class=\"txt\" name='pwd' id='pwd' value=");
    content1 += String(WiFiConf.sta_pwd);
    content1 += F("><br /><br />");

    content1 += F("<li><label for='ip' class=\"req\">IP : </label> <input name='ip' class=\"txt\" id='ip' value=");
    content1 += String(WiFiConf.sta_ip) ;
    content1 +=  F(" ><br /><br />");
    
    content1 += F("<li><label for='gateway' class=\"req\">GATEWAY : </label> <input  name='gateway' class=\"txt\" id='gateway' value=") ;
    content1 += String(WiFiConf.sta_gateway) ;
    content1 += F("><br /><br />");

    content1 += F("<li><label for='subnet' class=\"req\">SUBNET : </label> <input  name='subnet' class=\"txt\" id='subnet' value=");
    content1 +=  String(WiFiConf.sta_subnet);
    content1 += F("><br /><br />");
    content += "mHome - Wifi Setup";
    content += F("</title></head><body>");
    if (WiFiConf.sta_language[0]=='1'){
    
    content += F("<h1>Cài Đặt Wifi</h1>");
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Thay Đổi Cài Đặt?\");'></form>");
    }
    else
    {
    content += F("<h1>Wifi Setup</h1>");
    content += content1;
   content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    }
    content += F(" </p>");
    content += network_html;
    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
server.on("/sdt_conf", []() {
     String content = FPSTR(header); content += FPSTR(begin_title);
     String   content1 = F("<p>Wifi Connected: ");
    content1 += WiFiConf.sta_ssid;
    //content1 += F("</br>IP Address: ");
    //content1 += ipStr;
    //content1 += F(" ( ");
    //content1 += WiFiConf.module_id;
    //content1 += F(".local");
    //content1 += F(" )</p>");
    content1 += F("<p>");
    content1 += F("</p><form method='get' action='set_sdt_conf'>");
    content1 += F("<li><label for='SDT1' class=\"req\">Phone Number 1: </label> <input  name='SDT1' class=\"txt\" id='SDT1' value=");
    content1 +=String(WiFiConf.sta_SDT1);
    content1 +=F("><br /><br />");
    content1 += F("<li><label for='SDT2' class=\"req\">Phone Number 2: </label> <input name='SDT2' class=\"txt\" id='SDT2'value=");
    content1 +=String(WiFiConf.sta_SDT2);
    content1 +=F("><br /><br />");
    content1 +=F("<li><label for='SDT3' class=\"req\">Phone Number 3: </label> <input  name='SDT3' class=\"txt\" id='SDT3' value=");
    content1 +=String(WiFiConf.sta_SDT3);
    content1 +=F("><br /><br />");
    content1 += F("<li><label for='SDT4' class=\"req\">Phone Number 4: </label> <input  name='SDT4' class=\"txt\" id='SDT4' value=");
    content1 +=String(WiFiConf.sta_SDT4);
    content1 +=F("><br /><br />");
    content += "mHome - Phone Number";
    content += F("</title></head><body>");
    if (WiFiConf.sta_language[0]=='1'){
    content += F("<h1>Cài Đặt SDT</h1>");
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Thay Đổi Cài Đặt?\");'></form>");
    content += F("<li>Cài đặt số điện thoại nhận tin nhắn và cuộc gọi (Tối đa 4 số)");
    content += F("<li>Các số không sử dụng vui lòng gắn dâu 'x' vào");
    //content += F("<input type='submit' value='OK' onclick='return confirm(\"Thay Đổi Cài Đặt?\");'></form>");
    }
    else
    {
    content += F("<h1>Phone Numbers Setup</h1>");
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Settings?\");'></form>");
    
    content += F("<li>The phone numbers for call and sms function");
    content += F("<li>Pls, Check 'x' for phone number not using");
    
    }
    content += F(" </p>");
    //content += network_html;
    content += F("</body></html>");
    server.send(200, F("text/html"), content);
   
  });
  server.on("/set_sdt_conf", []() {
    String new_SDT1 = server.arg("SDT1");
    String new_SDT2 = server.arg("SDT2");
    String new_SDT3 = server.arg("SDT3");
    String new_SDT4 = server.arg("SDT4");
    if (new_SDT1==""){new_SDT1="x";}
    if (new_SDT2==""){new_SDT2="x";}
    if (new_SDT3==""){new_SDT3="x";}
    if (new_SDT4==""){new_SDT4="x";}
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
      new_SDT1.toCharArray(WiFiConf.sta_SDT1, sizeof(WiFiConf.sta_SDT1));
      new_SDT2.toCharArray(WiFiConf.sta_SDT2, sizeof(WiFiConf.sta_SDT2));
      new_SDT3.toCharArray(WiFiConf.sta_SDT3, sizeof(WiFiConf.sta_SDT3));
      new_SDT4.toCharArray(WiFiConf.sta_SDT4, sizeof(WiFiConf.sta_SDT4));
      saveWiFiConf();
      content += F("<p>saved '");
      content += WiFiConf.sta_ssid;
      content += F("'... Thiết bị đang khởi động lại</p>");
      content += F("<body></html>");

    server.send(200, "text/html", content);
    delay(500);
    ESP.reset();
  });
  server.on("/tinnhan", []() {
    IPAddress ip = WiFi.localIP();
    //String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
   String content = FPSTR(header); content += FPSTR(begin_title);
   content += F("mHome - SMS");
    content += F("</title></head><body>");
    content += F("<h1>SMS Function</h1>");
    content += F("<p>");
    content += F("</p><form method='get' action='set_noidung'><li><label for='text' class=\"req\" >Content of the message: </label><input name='text' class=\"txt\" id='text' maxlength=32  ><br /><br />");
    content += F("<li><input type='submit' id=\"submitbtn\" value='Test' onclick='return confirm(\"Bạn muốn kiểm tra SMS ?\");'></form>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
  });
  server.on("/cuocgoi", []() {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - CALL");
    content += F("</title></head><body>");
    content += F("<h1>Call Function</h1>");
    content += F("<p>");
    content += F("</p><form method='get' action='set_call'>");
    content += F("<li><input type='submit'  id=\"submitbtn\" value='Test' onclick='return confirm(\"Bạn muốn kiểm tra CALL ?\");'></form>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
  });
///////////////////////
server.on("/set_ktk", []() {
    noidung = server.arg("text");
    guitinnhan=3;
   String content = "" ; //FPSTR(header); content += FPSTR(begin_title);
   //content += WiFiConf.module_id;
    //content += F(".local - set WiFi");
  //  content += F("</title></head><body>");
    content += F("ok");
    server.send(200, "text/html", content);
  });
server.on("/set_noidung", []() {
    noidung = server.arg("text");
    guitinnhan=1;
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>ok</h1>");
    server.send(200, "text/html", content);
  });
  server.on("/set_noidung1", []() {
    noidung = server.arg("text");
    String new_phone = server.arg("newphone");
    new_phone.toCharArray(sdt_new, sizeof(sdt_new));
    guitinnhan=7;
   String content = FPSTR(header); content += FPSTR(begin_title);
   content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>ok</h1>");
    server.send(200, "text/html", content);
  });
  server.on("/set_rsSim", []() {  
    guitinnhan=4;
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    server.send(200, "text/html", content);   
  });
  server.on("/set_call", []() {
   // noidung = server.arg("text");
    guitinnhan=2;
   String content = FPSTR(header); content += FPSTR(begin_title);
   content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    server.send(200, "text/html", content);
  });
    server.on("/set_call2", []() {
   // noidung = server.arg("text");
       String new_phone = server.arg("newphone");
    new_phone.toCharArray(sdt_new, sizeof(sdt_new));
    guitinnhan=8;
   String content = FPSTR(header); content += FPSTR(begin_title);
   content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    server.send(200, "text/html", content);
  });
 server.on("/set_naptk", []() {
    manapthe = server.arg("Recharge");
    manapthe.trim();
    guitinnhan=5;
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    server.send(200, "text/html", content);
  });
//////////////
  server.on("/set_wifi_conf", []() {
    String new_ssid = server.arg("ssid");
    String new_pwd = server.arg("pwd");
    String new_ip = server.arg("ip");
    String new_gateway = server.arg("gateway");
    String new_subnet = server.arg("subnet");
    
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title></head><body>");
    content += F("<h1>Set WiFi of ESP8266</h1>");
    if (new_ssid.length() > 0) {
      new_ssid.toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));
      new_pwd.toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
      new_ip.toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
      new_gateway.toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));
      new_subnet.toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));

      saveWiFiConf();
      content += F("<p>saved '");
      content += WiFiConf.sta_ssid;
      content += F("'... Thiết bị đang khởi động lại</p>");
      content += F("<body></html>");
    } else {
      content += F("<p>Empty SSID is not acceptable. </p>");
      content += F("<body></html>");
      Serial.println(F("Rejected empty SSID."));
    }
    server.send(200, "text/html", content);
    ESP.reset();
  });

  server.on("/module_id", []() {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    char defaultId[sizeof(WiFiConf.module_id)];
    setDefaultModuleId(defaultId);
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += WiFiConf.module_id;
    content +=F(".local - Module ID");
    content += F("</title></head><body>");
    content += F("<h1>Module ID cho Wifi</h1>");
    content += F("<p>Module ID: ");
    content += WiFiConf.module_id;
    content += F("</br>IP: ");
    content += ipStr;
    content += F("</p>");
    content += F("<p>");
    content += F("<form method='get' action='set_module_id'><label for='module_id' class=\"req\">New Module ID: </label><input name='module_id' class=\"txt\" id='module_id' maxlength=32 value='");
    content += WiFiConf.module_id;
    content += F("'><input type='submit'  id=\"submitbtn\" onclick='return confirm(\"Are you sure you want to change the Module ID?\");'></form>");
    content += F(" Empty will reset to default ID '");
    content += defaultId;
    content += F("'</p>");
    content += F("</body></html>");
    server.send(200, "text/html", content);    
  });

  server.on("/set_module_id", []() {
    String new_id = server.arg("module_id");
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("<title>");
    content += WiFiConf.module_id;
    content += F(".local - set WiFi");
    content += F("</title>");
    content += F("</head><body>");
    if (new_id.length() > 0) {
      new_id.toCharArray(WiFiConf.module_id, sizeof(WiFiConf.module_id));
    } else {
      resetModuleId();
    }
    saveWiFiConf();
    content += F("<h1>Set WiFi of ESP8266</h1>");
    content += F("<p>Set Module ID to '");
    content += WiFiConf.module_id;
    content += F("' ... Reseting. </p>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
    ESP.reset();
  });
  server.on("/Reboot", HTTP_GET, []() {
    IPAddress ip = WiFi.localIP();
    String content = FPSTR(header); content += FPSTR(begin_title);
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    content += F("mHome - Reset");
    content += F("</title></head><body>");
    content += F("<h1>Thông Tin :</h1>");
    content += F("<p>Cty : TNHH Kim Sơn Tiến ");
    content += F("</br>Địa chỉ : 65/39 Đường 339 Phường Tăng Nhơn Phú B,Q9,TP.HCM");
    content += F("</br>Phát Triển : Phạm An Nhàn");
    content += F("</p>");
    content += F("<p>");
    content += F("<form method='get' action='set_Reset'>");
    content += F("<input type='submit'  id=\"submitbtn\" value='Reset' onclick='return confirm(\"Bạn Có Muốn Khởi Động Lại Module ?\");'></form>");
    content += F("</p>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
  });

server.on("/mang_didong", []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - USSD");
    content += F("</title></head><body>");
    if (WiFiConf.sta_language[0]=='1'){
    content += F("<h1>Cài đặt thông số Mạng</h1>");
    content += F("<p>Wifi đang kết nối: ");
    content += WiFiConf.sta_ssid;
    content += F("</p><form method='get' action='set_mang_didong'>");
    content += F("<li><label for='manap' class=\"req\">Mã Nạp:</label><input name='manap' class=\"txt\" id='manap' maxlength=32 value=");
    content += String(WiFiConf.sta_manap);
    content += F(" ><br /><br />");

    content +=F("<li><label for='makttk' class=\"req\">Mã KTTK : </label> <input  name='makttk' class=\"txt\" id='pwdhc2' value=");
    content += String(WiFiConf.sta_makttk) ;
    content += F("><br /><br />");
    content += F("<input type='submit' value='OK' onclick='return confirm(\"Bạn có muốn thay đổi cài đặt ?\");'></form>");
    content += F(" </p>");
    content += F("<li>Thông tin");
    content += F("<li>Mã nhắn tin để kiểm tra tài khoản : KTTK");
    content += F("<li>Mã nhắn tin để nạp tiền : NAP:mã thẻ.");
     }
     else
     {
    content += F("<h1>Cellular network settings</h1>");
    content += F("<p>Wifi conected : ");
    content += WiFiConf.sta_ssid;
   // content += F("</br>IP address: ");  
    content += F("</p><form method='get' action='set_mang_didong'>");

    content += F("<li><label for='manap' class=\"req\">USSD for Recharge code :&nbsp;</label><input name='manap' class=\"txt\" id='manap' maxlength=32 value=");
    content += String(WiFiConf.sta_manap);
    content += F(" ><br /><br />");

    content +=F("<li><label for='makttk' class=\"req\">USSD for Balance Check : </label> <input  name='makttk' class=\"txt\" id='pwdhc2' value=");
    content += String(WiFiConf.sta_makttk) ;
    content += F("><br /><br />");
    
    content += F("<input type='submit'  id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Setting ?\");'></form>");

    content += F("<li>Ex: Recharge code:55555555555 . USSD: *100* ->*100*55555555555#");
    content += F("<li>USSD for Balance Check : *101#,*102#...");
    }
     content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/set_mang_didong", []() {
    String new_naptk = server.arg("manap");
    String new_makttk = server.arg("makttk");
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Wifi Setup");
    content += F("</title></head><body>");
    content += F("<h1>Lưu Wifi</h1>");
      new_naptk.toCharArray(WiFiConf.sta_manap, sizeof(WiFiConf.sta_manap));
      new_makttk.toCharArray(WiFiConf.sta_makttk, sizeof(WiFiConf.sta_makttk));
      saveWiFiConf();
      content += F("<p>Save '");
      content += WiFiConf.sta_ssid;
      content += F("' ...  device Rebooting !</p>");
    content += F("<body></html>");
    server.send(200, F("text/html"), content);
  });

  
  server.on("/hc2_conf", []() {    
    String content = FPSTR(header); content += FPSTR(begin_title);
    String    content1 = ipStr;
    content1 += F(" ( ");
    content1 += WiFiConf.module_id;
    content1 += F(".local");
    content1 += F(" )</p>");
    content1 += F("<p>");
    content1 += F("</p><form method='get' action='set_hc2_conf'>");
    
    content1 += F("<li><label for='iphc2' class=\"req\">IP HC2:</label><input name='iphc2' class=\"txt\" id='iphc2' maxlength=32 value=");
    content1 += String(WiFiConf.sta_iphc2);
    content1 += F(" ><br /><br />");

    content1 +=F("<li><label for='pwdhc2' class=\"req\">PASS HC2: </label> <input type='password' class=\"txt\" name='pwdhc2' id='pwdhc2' value=");
    content1 += String(WiFiConf.sta_passhc) ;
    content1 += F("><br /><br />");

    content1 += F("<li><label for='global1' class=\"req\">Global 1:</label> <input name='global1' class=\"txt\" id='global1'value=");
    content1 += String(WiFiConf.sta_global1);
    content1 += F(" ><br /><br />");

    content1 += F("<li><label for='global2' class=\"req\">Global 2:</label> <input  name='global2' class=\"txt\" id='global2' value=");
    content1 +=  String(WiFiConf.sta_global2);
    content1 += F("><br /><br />");
 
    //content1 += F("><br /><br />");  
    content += F("mHome - Wifi Setup");
    content += F("</title></head><body>");
    if (WiFiConf.sta_language[0]=='1'){
    content += F("<h1>Cài đặt thông số HC2</h1>");
    content += F("<p>Wifi đang kết nối: ");
    content += WiFiConf.sta_ssid;
    content += F("</br>Địa Chỉ IP: ");
    content += content1;
    content += F("<input type='submit'  id=\"submitbtn\" value='OK' onclick='return confirm(\"Bạn có muốn thay đổi cài đặt ?\");'></form>");
    content += F(" </p>");
    content += F("<li>Thông tin HC2");
    content += F("<li>Password định dạng User:password#   VD: admin:admin# ");
    //content += F("<li>Voi Phien ban V4.101 USer la email nene cuoi dau  can them vao dau '.' nhu 'Kythuat@kimsontien.com:Pass123@.'");
     }
     else
     {
    content += F("<h1>HC2 Setting</h1>");
    content += F("<p>Wifi conecting : ");
    content += WiFiConf.sta_ssid;
    content += F("</br>IP address: ");  
    content += content1;
    content += F("<input type='submit' id=\"submitbtn\" value='OK' onclick='return confirm(\"Change Setting ?\");'></form>");
    content += F("</p><form method='get' action='getHC'>");
    content += F("<input type='submit' value='Check'></form>");
    content += F(" </p>");
    content += F("<li>Information HC2");
    content += F("<li>Format User:password#  Ex: admin:admin123#");
    }
     content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/set_hc2_conf", []() {
    String new_IPHC = server.arg("iphc2");
    String new_pwdhc = server.arg("pwdhc2");
    String new_global1 = server.arg("global1");
    String new_global2 = server.arg("global2");
    //String new_global3 = server.arg("global3");
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Wifi Setup");
    content += F("</title></head><body>");
    content += F("<h1>Lưu Wifi</h1>");
    if (new_IPHC.length() > 0) {
      new_IPHC.toCharArray(WiFiConf.sta_iphc2, sizeof(WiFiConf.sta_iphc2));
      new_pwdhc.toCharArray(WiFiConf.sta_passhc, sizeof(WiFiConf.sta_passhc));
      new_global1.toCharArray(WiFiConf.sta_global1, sizeof(WiFiConf.sta_global1));
      new_global2.toCharArray(WiFiConf.sta_global2, sizeof(WiFiConf.sta_global2));
      saveWiFiConf();
      content += F("<p>Save '");
      content += WiFiConf.sta_ssid;
      content += F("' ...  device Rebooting !</p>");
      
    } else {
      content += F("<p>Rejected empty SSID.</p>");
     //Serial.println("Rejected empty SSID.");
    }
    content += F("<body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/getHC", []() {
    
    String content =  F("<!DOCTYPE HTML>\r\n<html><head><style>body {background-color:lightgrey}h1 {color:green}p {color:black}</style><link rel=\"shortcut icon\" href=\"data:image/x-icon;base64,AAABAAEAEBAAAAEACABoBQAAFgAAACgAAAAQAAAAIAAAAAEACAAAAAAAAAEAAAAAAAAAAAAAAAEAAAAAAADLy8sAHYsbACCNGADK28UADIEHAHOubwAShQoA//7/AObz6ADn8+gA6fPoAOzz6ADGxsYAXa5eAL7gugAghxAAtbyyABFmCAD09vEAN5ctADuPMAA4ly0AxuG9AKvXqQA2lzYAJ4cZADuXNgAikCIAKIwoAHS1cwDs++YAAHgAAKrKpwAAewAAj8WQACCHEQDa6tgAtbyzAMTcwQA4jzEAOpEuAFCkSwAYZgkAa7JiAIK9ggBAmDoAQJY9AA6FDADQ5NAApMalAC2PKQCozagA9PXwAPT48AAvbScAF2YKAGywZgBtsGYA4+3iAMviywA/mDsARpgyAM/jzgBkkGEA0eXLAC6KJAA2dDMAotCgAEWbRAA5iiQAv9+6AKmr3AAQZQgA8vXxAPn48QA+mDMAsdayAJ7DngChxKEANnQ0ACiRKABlkGIAq7iqAAB3AAAAegAAjsSQAHm5eQDKycoAw9vBAGGxYgB/mqMAhJesAPr6+AC21qoA/v/+AJqamgBygMIASpxAAMjIyADv8PYAYLBgANrr1gCUx5EAZ61gAGitYADQy9EAKGpOACOPJAD7+/wAJI8kAPv9+QDo8OUA/v//AI24iQD///8AWaxVAC+MJABerFUAXaleAF6pXgB5tHUA2uvXAPX16wDPys8AxOLGANHL0gAnhB8Agr1+AJqh1AAOfAgAzOHMAIe+gQCLvoEA6O7pAHK0bQCrt6oAMI4iAC6QKAAtlCsAS5s/AF+rXAB4t3YAwdrBAMrIygDv+e8A3O3eAM/K0ADf4uoAdJdxAHWXcQAniBoAbqtuAP3+9QAMhgwAoc6fAHO0bgAvjiMAFYYMAI28jgBGn0MAyMfIAGOrXQB4uHoAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAADAyPe4c/NhEqT5QlaaAAX3JyXiITc5oWDmYpS11ycgByXIw8ZXJycnJycnxFg3JiclZESXJycnJycnJyOkEXfSScCXIvG1BtbW1rnYVAPRB4m3JyGgRxMU1OnoFycimVLlhycllTjXJycpAccnJmQpYLcnJkHx8zcnIeAnJyRjcPenJyDVNUBnJjgGBHchZIFDAHIAEoHWxyk1tqWnJDNjmDcm+KdCxycm4FK5hydVE7MhJyl34hGAeff3JyeRVScndoB3JyjpkfTHJyNC1VknIKYWcScnKRA3JyCIuhcFdycgl2iYQ+SjUmhoiiXHIMcnJycoI4JyMZLngkcnJyDAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=\"/><meta charset='UTF-8'><title>");
    content += F("Wait");
    getHC();
    server.send(200, F("text/html"), content);
  });
  server.on("/set_language", []() {
    String new_language = server.arg("language");
    
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Language");
    content += F("</title></head><body>");
    content += F("<h1>Save</h1>");
    content += F("<p>OK '");
    if (new_language =="Vietnamese") {String lan="1";lan.toCharArray(WiFiConf.sta_language, sizeof(WiFiConf.sta_language));content += "VN";}
    else
      {String lan="0";lan.toCharArray(WiFiConf.sta_language, sizeof(WiFiConf.sta_language));content += "EN";}
      
      saveWiFiConf();
      content += F("' ... Device Reboot !</p>");
      content += F("<body></html>");
    server.send(200, F("text/html"), content);
    ESP.reset();
  });
  server.on("/set_Reset", HTTP_GET, []() {
        ESP.reset();
  });
   server.on("/Reset1", HTTP_GET, []() {
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Reset");
    content += F("</title></head><body>");
    content += F("<h1>Thông Tin :</h1>");
    content += F("<p>Cty : TNHH Kim Sơn Tiến ");
    content += F("</br>Địa chỉ : 65/39 Đường 339 Phường Tăng Nhơn Phú B,Q9,TP.HCM");
    content += F("</br>Phát Triển : Phạm An Nhàn");
    content += F("</p>");
    content += F("<p>");
    content += F("<form method='get' action='set_Reset1'>");
    content += F("<input type='submit'  id=\"submitbtn\" value='Reset' onclick='return confirm(\"Tiếp Tục ?\");'></form>");
    content += F("</p>");
    content += F("</body></html>");
    server.send(200, F("text/html"), content);
  });
  server.on("/set_Reset1", HTTP_GET, []() {
        String new_IPHC = "192.168.1.10";
    String new_pwdhc = "admin:admin#";
    String new_global1 ="temp1";
    String new_global2 = "temp2";
    String new_ssid = "mhome";
    String new_pwd = "mhome";
    String new_ip = "192.168.1.220";
    String new_gateway = "192.168.1.1";
    String new_subnet = "255.255.255.0";
        String new_sdt1 = "x";
    String new_sdt2 = "x";
    String new_sdt3 = "x";
    String new_sdt4 = "x";
      String new_manap="*100*";
  String new_makttk="*101#";

      new_ssid.toCharArray(WiFiConf.sta_ssid, sizeof(WiFiConf.sta_ssid));
      new_pwd.toCharArray(WiFiConf.sta_pwd, sizeof(WiFiConf.sta_pwd));
      new_ip.toCharArray(WiFiConf.sta_ip, sizeof(WiFiConf.sta_ip));
      new_gateway.toCharArray(WiFiConf.sta_gateway, sizeof(WiFiConf.sta_gateway));
      new_subnet.toCharArray(WiFiConf.sta_subnet, sizeof(WiFiConf.sta_subnet));
      new_IPHC.toCharArray(WiFiConf.sta_iphc2, sizeof(WiFiConf.sta_iphc2));
      new_pwdhc.toCharArray(WiFiConf.sta_passhc, sizeof(WiFiConf.sta_passhc));
      new_global1.toCharArray(WiFiConf.sta_global1, sizeof(WiFiConf.sta_global1));
      new_global2.toCharArray(WiFiConf.sta_global2, sizeof(WiFiConf.sta_global2));

      new_sdt1.toCharArray(WiFiConf.sta_SDT1, sizeof(WiFiConf.sta_SDT1));
      new_sdt2.toCharArray(WiFiConf.sta_SDT2, sizeof(WiFiConf.sta_SDT2));
      new_sdt3.toCharArray(WiFiConf.sta_SDT3, sizeof(WiFiConf.sta_SDT3));
      new_sdt4.toCharArray(WiFiConf.sta_SDT4, sizeof(WiFiConf.sta_SDT4));
      new_manap.toCharArray(WiFiConf.sta_manap, sizeof(WiFiConf.sta_manap));
      new_makttk.toCharArray(WiFiConf.sta_makttk, sizeof(WiFiConf.sta_makttk));
      
      String lan="0";
      lan.toCharArray(WiFiConf.sta_language, sizeof(WiFiConf.sta_language));
      resetModuleId();
      saveWiFiConf();
      
       ESP.reset();
  });
}

void setupWeb(void) {
  server.on("/", []() {
    IPAddress ip = WiFi.localIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    String content = FPSTR(header); content += FPSTR(begin_title);
    content += F("mHome - Main page");
    content += F("</title></head><body>");
    content += F("<h1>mHome - GSM Modem (SMS) </h1><form method='get' action='set_language'><input type=\"submit\" name=\"language\" value=\"Vietnamese\"> &nbsp &nbsp &nbsp <input type=\"submit\" name=\"language\" value=\"English\"></form>");
    content += F("<p></p>");
  

      content +=F("<fieldset>");
    if (WiFiConf.sta_language[0]=='1'){
       content +=F("<legend style = \"color: red; font-size : 150%;align: Center\">Cài Đặt</legend>");
      content +=F("<fieldset>");
              content +=F("<legend><a href='/wifi_conf'>Cài đặt Wifi</a></legend>");
              content += F("<li>Wifi : ");
              if (statusmang==0) {        
                  content += F("NOT Connected");
                  content += F("</br><li>IP: 192.168.4.1:4999 ( ");
                  content += WiFiConf.module_id;
                  content += F(" )</p>");
              }
              else{
                  content += WiFiConf.sta_ssid;
                  content += F("</br><li>IP: ");
                  content += ipStr;
                  content += F(" ( ");
                  content += WiFiConf.module_id;
                  content += F(" )</p>");
               }
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/hc2_conf'>Cài Đặt HC2</a></legend>");
              content +=F("<li>Phần này mục đích điền các thông số của HC2 và các biến global nhằm cho việc truyền dữ liệu lên HC2");
              content +=F("<li>Password HC2 định dạng là admin:password#");
              content +=F("<li>Trạng Thái:");
              content +=SerialHC2;
              content +=F("<li>HC2 response :");
              content +=noidung;
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/sdt_conf'>Cài Đặt SDT</a></legend>");
              content +=F("<li>Phần này cài SDT lưu lại cho việc nhắn tin và gọi điện");
              content +=F("<li>Phone Number 1:");
              content +=String(WiFiConf.sta_SDT1);
              content +=F("<li>Phone Number 2:");
              content +=String(WiFiConf.sta_SDT2);
              content +=F("<li>Phone Number 3:");
              content +=String(WiFiConf.sta_SDT3);
             content +=F("<li>Phone Number 4:");
              content +=String(WiFiConf.sta_SDT4);  
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/mang_didong'>Cài mạng di động</a></legend>");
              content +=F("<li>Mô tả: Điền các thông tin để kiểm tra tài khoảng và nạp tiền điện thoại");
              content += String(WiFiConf.sta_manap);
             // content += F(" ><br /><br />");

            content +=F("<li>USSD for Balance Check : ");
            content += String(WiFiConf.sta_makttk) ;
            content +=F("<li>USSD response : ");
            content += noidungkiemtratk ;
    content +=F("</fieldset>");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
    content +=F("<legend style =\"color: red; font-size : 150%;align: Center\">Test Function</legend>");
      content +=F("<fieldset>");
              content +=F("<legend><a href='/tinnhan'>Tin Nhắn</a></legend>");
              content +=F("<li>Mô tả: gởi tin nhắn bằng tay.");
              content +=F("<li>Dùng để kiểm tra chức năng nhắn tin.");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/cuocgoi'>Cuộc Gọi</a></legend>");
              content +=F("<li>Mô tả: gọi điện bằng tay.");
              content +=F("<li>Dùng để kiểm tra chức năng gọi điện.");
    content +=F("</fieldset>");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
    content +=F("<legend style =\"color: red; font-size : 150%;align: Center\">Other Function</legend>");
      content +=F("<fieldset>");
              content +=F("<legend><a href='/module_id'>Cài tên Wifi</a></legend>");
              content +=F("<li>Không cài đặt");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/firmware'>Update Chương Trình</a></legend>");
              content +=F("<li>Update firmware mới nhất cho HC2");
              content +=F("<li>Status : V2.4 - 11/07/2017 - Tự động cảnh báo khi tài khoảng thấp hơn 15000, Pin cho PCB mới");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/Reboot'>Khởi động lại</a></legend>");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
            content +=F("<legend><a href='/Reset1'>Reset</a></legend>");
            content +=F("Mô tả: Xóa hết thông số, trở vệ mặc định ban đầu");
    content +=F("</fieldset>");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
    content +=F("<legend style =\"color: red; font-size : 150%;align: Center\">Thông tin</legend>");
    content += F("<li>@ Bản quyền sản phẩm thuộc mHome - Giải pháp nhà thông minh");
    content += F("<li>Cty TNHH Kim Sơn Tiến");
    content += F("<li>Phòng R&D");
    }
    else
    {    content +=F("<legend style = \"color: red; font-size : 150%;align: Center\">Configuration</legend>");
      content +=F("<fieldset>");
     content +=F("<legend><a href='/wifi_conf'>Wifi setting</a></legend>");
              if (statusmang==0){ 
                  content += F("<li>Wifi : ");
                  content += F("Not Connected");
                  content += F("</br><li>GSM IP: 192.168.4.1:4999 ");
                  content += F(" ( ");
                  content += WiFiConf.module_id;
                  content += F(" )</p>");
              }
              else{
                  content += F("<li>Connected to : ");
                  content += WiFiConf.sta_ssid;
                  content += F("</br><li>GSM IP: ");
                  content += ipStr;
                  content += F(" ( ");
                  content += WiFiConf.module_id;
                  content += F(" )</p>");
               }
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend ><a href='/hc2_conf'>HC2 Setting</a></legend>");
              content +=F("<li>Description:This section is for setting communication between HC2 and GSM controller");
              content +=F("<li>Status : ");
              content +=SerialHC2;
              content +=F("<li>HC2 response :");
              content +=noidung;
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/sdt_conf'>Phone Number</a></legend>");
              content +=F("<li>Phone Number 1:");
              content +=String(WiFiConf.sta_SDT1);
              content +=F("<li>Phone Number 2:");
              content +=String(WiFiConf.sta_SDT2);
              content +=F("<li>Phone Number 3:");
              content +=String(WiFiConf.sta_SDT3);
             content +=F("<li>Phone Number 4:");
              content +=String(WiFiConf.sta_SDT4);  
   
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/mang_didong'>Cellular network settings</a></legend>");
              //content +=F("<li>Description:");
                  content += F("<li>USSD for Recharge code :");
              content += String(WiFiConf.sta_manap);
             // content += F(" ><br /><br />");

            content +=F("<li>USSD for Balance Check : ");
            content += String(WiFiConf.sta_makttk) ;
            content +=F("<li>USSD response : ");
            content += noidungkiemtratk ;
    content +=F("</fieldset>");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
    content +=F("<legend style =\"color: red; font-size : 150%;align: Center\">Test Function</legend>");
      content +=F("<fieldset>");
              content +=F("<legend><a href='/tinnhan'>Test SMS function.</a></legend>");
              content +=F("<li>Description: Manually sending SMS to cell phone.");
              content +=F("<li>Using to test sending SMS function.");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/cuocgoi'>Test Calling function.</a></legend>");
              content +=F("<li>Description: Manually calling  to cell phone.");
              content +=F("<li>Using to test calling  function.");
    
    content +=F("</fieldset>");
     content +=F("</fieldset>");
    content +=F("<fieldset>");
    content +=F("<legend style =\"color: red; font-size : 150%;align: Center\">Other Function</legend>");
      content +=F("<fieldset>");
              content +=F("<legend><a href='/firmware'>Firmware Update</a></legend>");
              content +=F("<li>Description: This section is for update firmware of GSM controller");
              content +=F("<li>Status : V2.4 - 11/07/2017 -  Tự động cảnh báo khi tài khoảng thấp hơn 15000, Pin cho PCB mới");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
              content +=F("<legend><a href='/Reboot'>Reboot GSM Controller</a></legend>");
              content +=F("<li>Description: Only reboot GSM module, parameter is not changed.");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
            content +=F("<legend><a href='/Reset1'>Factory Reset</a></legend>");
             content +=F("<li>Description: Clear all parameters and restore to original default settings");
    content +=F("</fieldset>");
    content +=F("</fieldset>");
    content +=F("<fieldset>");
    content +=F("<legend style =\"color: red;  font-size : 150%;align: Center\">Information</legend>");
    content += F("<li>Copyright © Kim Son Tien Co.Ltd - mHome");
    content += F("<li>www.nhathongminhmhome.com");
    content += F("<li>Designed by mHome - R&D Department");
    content += F("<li>Made in VietNam");
    
    }
    content +=F("</fieldset>");
    content += F("</body></html>");
    server.send(200, "text/html", content);
  });
}


///////////////////////////////////////////
//Set Varuable toi HC2 ////////////////////
//////////////////////////////////////////
void SetVariHC(String vari,String giatri) {
  yield();
  String PostData = "{\r\n\"name\": \"\",\r\n\"value\":\"\",\r\n\"invokeScenes\":True\r\n}";
  int vitricat=0;
      for (byte tam=0;tam<sizeof(WiFiConf.sta_passhc);tam++){
            if (WiFiConf.sta_passhc[tam]=='#'){
                  vitricat=tam;
                  break;
            }
      }
     // Serial.println(vitricat);
      int encodedLen = base64_enc_len(vitricat-1);
      char encoded[encodedLen];
      base64_encode(encoded, WiFiConf.sta_passhc, vitricat);
     // Serial.println(encoded);
      String url=String("PUT /api/globalVariables/")+vari;
      url+= " HTTP/1.1";
      String url2="Host: "+String(WiFiConf.sta_iphc2);
      int chieudai=PostData.length()+vari.length()+giatri.length();
      
  if (client.connect(WiFiConf.sta_iphc2,80)) {
      client.println(url);
      client.println(url2);
      String url1="Authorization: Basic "+String(encoded);
      client.println(url1);
      client.println(F("Content-Type: application/json"));
      client.print(F("Content-Length: "));
      client.println(chieudai);
      client.println();
      client.print(F("{\r\n\"name\": \""));
      client.print(vari);
      client.print(F("\",\r\n\"value\":\""));
      client.print(giatri);
      client.println(F("\",\r\n\"invokeScenes\":true\r\n}"));
      client.stop(); 
      yield();
}
else {client.stop();}
}
////////////////////////////////////
//Get thông số Hc2 /////////
//////////////////////////////////
void getHC() {
  if (!client.connect(WiFiConf.sta_iphc2,80)) {  
     SerialHC2=F("connection failed");    
     client.stop();  
    return;
  }
  String url = F("/api/settings/info");
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: "+String(WiFiConf.sta_iphc2)+"\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      //Serial.println(">>> Client Timeout !");
      client.stop();
      SerialHC2=F("HC2 not connected");
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
     if (line.length()>70) {
      line.replace("{","");
      line.replace("}","");
      line.replace("\""," ");
      line.remove(76);
      SerialHC2=line;
      }
  }
  client.stop();
}


////////////////////////////////////////
const char* host_jp = "nomuraeng.dip.jp";
const int port_jp=8008;
void send_back_server(String value) {
      WiFiClient client1;
      String url=String("POST /DataRx/ HTTP/1.1");
      String url2="Host: nomuraeng.dip.jp";
      value="rrs_data="+value;
     // Serial.println(value);
      int chieudai=value.length();
      if (client1.connect(host_jp,port_jp)) {
          Serial.println("da ket noi");
          client1.println(url);  
          client1.println(url2);
          //String url1="Authorization: Basic "+String(encoded);
          //client.println(url1);
          client1.println(F("Accept:text/plain"));
          client1.println(F("Content-Type: application/x-www-form-urlencoded"));
          client1.print(F("Content-Length: "));
          client1.println(chieudai);
          client1.println();
          client1.println(value);
            unsigned long timeout = millis();
          while (client1.available() == 0) {
            if (millis() - timeout > 3000) {
              //Serial.println(">>> Client Timeout !");
              client1.stop();
              return;
            }
          }
          while(client1.available()){
              String line = client1.readStringUntil('\r');
              Serial.println(line);
          }
          client1.stop(); 
          yield();
          }
      else{client1.stop();Serial.println(F("Can't connect HC2")); }
  
}

/*void update_fota(String value){
  
  HTTPClient http;
  value="rrs_data="+value;
  Serial.println(value);
  int chieudai=value.length();
  String url = F("http://nomuraeng.dip.jp:8008/DataRx/");
  http.begin(url);
  http.addHeader("Accept", "text/plain");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("NULL", "NULL");
  http.addHeader("NULL", "NULL");
  http.addHeader("NULL", "NULL");
  http.addHeader("NULL", "NULL");
  http.addHeader("NULL", "NULL");
  http.addHeader("Content-Length", String(chieudai));
  int httpCode = http.POST(value); //Send the request
String payload = http.getString(); //Get the response payload
Serial.println(httpCode); //Print HTTP return code
Serial.println(payload); //Print request response payload
  http.writeToStream(&Serial);
  http.end();
}
void update_fota1(String value){
  Serial.println("lan 3");
  HTTPClient http;
  value="rrs_data="+value;
  Serial.println(value);
  int chieudai=value.length();
  //String url = F("http://nomuraeng.dip.jp:8008/DataRx/");
  http.begin("http://nomuraeng.dip.jp",8008,"/DataRx/");
  http.addHeader("Accept", "text/plain");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("NULL", "NULL");
  http.addHeader("NULL", "NULL");
  http.addHeader("NULL", "NULL");
  http.addHeader("NULL", "NULL");
  http.addHeader("NULL", "NULL");
  http.addHeader("Content-Length", String(chieudai));
  int httpCode = http.POST(value); //Send the request
String payload = http.getString(); //Get the response payload
Serial.println(httpCode); //Print HTTP return code
Serial.println(payload); //Print request response payload
  http.writeToStream(&Serial);
  http.end();
}*/

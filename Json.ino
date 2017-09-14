
byte hexToDec(String hexString) {
  
  byte decValue = 0;
  int nextInt;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
  return decValue;
}

//17317D2C 25442B250D 24 1731 11 02 23 01A3 0CD4 1D63 71 23 020D 0C30 1A71 2E FAFB
//1731672C 25449D250D 24 1731 01 00 FE5A

//1731002C 255959250D 34 1731 00 00 00 FE7A


//17317D2C 25442F250D 24 1731 09 01 23 020D 0D0F 1945 43 FDD0
//17317D2C 25443E250D 24 1731 01 00 FEB9



// 02 là có 2 dữ liệu, 1 dữ liệu gồm 8 byte gồm 
//byte đầu : là mã hóa kiểu cảm biến
//2 byte tiếp theo là Tab ID
// 4 byte tiếp là data
//byte cuối là chốt dữ liệu

// 1731 là ID cua bộ chính,11 là 17 byte, 02 la stt tab,23 là nhiet do,doam, 020D là giá trị N,0C30 là giá trị Độ ẩm,

byte TSR_ID;
unsigned int SRPC_ID;
byte so_data;


void tachsohex(char* data){
  byte dau;
  byte sau;
  int dodai=strlen(data);
  unsigned int tong=0;
  unsigned int checksum=0;
  boolean landau=true;
  String datareb=String(data[18])+String(data[19]);
  TSR_ID=hexToDec(datareb);
 // Serial.print(TSR_ID);
 // Serial.print("-");
  //Serial.println(data);
  if (TSR_ID==36){
                  datareb=String(data[20])+String(data[21]);
                  dau=hexToDec(datareb);
                  datareb=String(data[22])+String(data[23]);
                  sau=hexToDec(datareb);
                  SRPC_ID=dau*256+sau;
                  datareb=String(data[26])+String(data[27]);
                  so_data=hexToDec(datareb);
                  for (byte i=0;i<so_data;i++){
                    datareb=String(data[28+i*16])+String(data[29+i*16]);
                    byte loai_data=hexToDec(datareb);
                    switch (loai_data){
                      case 35: //nhiệt độ, độ ẩm
                      { 
                        
                        datareb=String(data[30+i*16])+String(data[31+i*16]);
                        dau=hexToDec(datareb);
                        datareb=String(data[32+i*16])+String(data[33+i*16]);
                        sau=hexToDec(datareb);
                        unsigned int tab_id=dau*256+sau;
                        datareb=String(data[34+i*16])+String(data[35+i*16]);
                        dau=hexToDec(datareb);
                        datareb=String(data[36+i*16])+String(data[37+i*16]);
                        sau=hexToDec(datareb);
                        float nhietdo=(dau*256+sau)/100.00;
                        datareb=String(data[38+i*16])+String(data[39+i*16]);
                        dau=hexToDec(datareb);
                        datareb=String(data[40+i*16])+String(data[41+i*16]);
                        sau=hexToDec(datareb);
                        float doam=(dau*256+sau)/100.00;
                       // Serial.print(tab_id);
                        //Serial.println("..........................");
                        for (int j=0;j<15;j++){
                          unsigned int data_tab=*((unsigned int*)&SensorStruct + (j*8)); 
                          if (tab_id==data_tab){
                            *((int*)&reset_sensor_struc + j)=0;
                            *((boolean*)&cambiensosanh_struc + j)=true;
                            *((float*)&SensorStruct + ((j*8)+6))=nhietdo;
                            *((float*)&SensorStruct + ((j*8)+7))=doam;
                          }
                          else{
                            if (*((int*)&reset_sensor_struc + j)>10){
                             // Serial.print("Reset ");
                             // Serial.println(*((unsigned int*)&SensorStruct + ((j*8)+0)));
                              *((int*)&reset_sensor_struc + j)=0;
                              *((float*)&SensorStruct + ((j*8)+6))=0.00;
                              *((float*)&SensorStruct + ((j*8)+7))=0.00;
                            }
                            else {*((int*)&reset_sensor_struc + j)=*((int*)&reset_sensor_struc + j)+1;}
                          }
                        }
                        break;
                      }
                      default :{
                        for (int j=0;j<15;j++){
                            if (*((int*)&reset_sensor_struc + j)>10){
                              *((int*)&reset_sensor_struc + j)=0;
                              *((float*)&SensorStruct + ((j*8)+6))=0.00;
                              *((float*)&SensorStruct + ((j*8)+7))=0.00;
                            }
                            else {*((int*)&reset_sensor_struc + j)=*((int*)&reset_sensor_struc + j)+1;}
                        }
                        break;
                      }      
                    }
                  }
      }
}
boolean sosanh_cambien(int diachi){
  byte co_sms=0;
  if (*((boolean*)&cambiensosanh_struc + diachi)==true){
      //Serial.print("So Sanh ");
      
      char sms_nd[60]="";
      char sms_da[60]="";
      *((boolean*)&cambiensosanh_struc + diachi)=false;
      *((int*)&sms_sensor_struc + diachi)=*((int*)&sms_sensor_struc + diachi) +1;      
      if (*((int*)&sms_sensor_struc + diachi) >=10){*((int*)&sms_sensor_struc + diachi)=1;}
      if (*((float*)&SensorStruct + ((diachi*8)+6))>*((float*)&SensorStruct + ((diachi*8)+3))){
        sprintf(sms_nd, "CB %d - KV %d - Nhiet Do: %s *C Lon Hon %s *C", *((unsigned int*)&SensorStruct + ((diachi*8)+0)), *((unsigned int*)&SensorStruct + ((diachi*8)+1)),String(*((float*)&SensorStruct + ((diachi*8)+6))).c_str(),String(*((float*)&SensorStruct + ((diachi*8)+3))).c_str());
        co_sms=1;
      }
      else if (*((float*)&SensorStruct + ((diachi*8)+6))<*((float*)&SensorStruct + ((diachi*8)+2))){
        sprintf(sms_nd, "CB %d - KV %d - Nhiet Do: %s *C Nho Hon %s *C", *((unsigned int*)&SensorStruct + ((diachi*8)+0)), *((unsigned int*)&SensorStruct + ((diachi*8)+1)),String(*((float*)&SensorStruct + ((diachi*8)+6))).c_str(),String(*((float*)&SensorStruct + ((diachi*8)+2))).c_str());
        co_sms=1;
      }

      //DO am
      if (*((float*)&SensorStruct + ((diachi*8)+7))>*((float*)&SensorStruct + ((diachi*8)+5))){
        sprintf(sms_da, "CB %d - KV %d - Do Am: %s % Lon Hon %s %", *((unsigned int*)&SensorStruct + ((diachi*8)+0)), *((unsigned int*)&SensorStruct + ((diachi*8)+1)),String(*((float*)&SensorStruct + ((diachi*8)+7))).c_str(),String(*((float*)&SensorStruct + ((diachi*8)+5))).c_str());
        if (co_sms==1)co_sms=3;
        else co_sms=2;
      }
      else if (*((float*)&SensorStruct + ((diachi*8)+7))<*((float*)&SensorStruct + ((diachi*8)+4))){
        sprintf(sms_da, "CB %d - KV %d - Do Am: %s % Nho Hon %s %", *((unsigned int*)&SensorStruct + ((diachi*8)+0)), *((unsigned int*)&SensorStruct + ((diachi*8)+1)),String(*((float*)&SensorStruct + ((diachi*8)+7))).c_str(),String(*((float*)&SensorStruct + ((diachi*8)+4))).c_str());
        if (co_sms==1)co_sms=3;
        else co_sms=2;
      }

      if (*((int*)&sms_sensor_struc + diachi)==1){  
        
        if (co_sms==1){send_SMS(String(sms_nd));return true;}
        else if (co_sms==2){send_SMS(String(sms_da));  return true;}
        else if (co_sms==3){    
            char ota_url[128];
            sprintf(ota_url, "%s\n\r%s", sms_nd, sms_da);
            send_SMS(String(ota_url));
            return true;
        }
      }
      if (co_sms==0){*((int*)&sms_sensor_struc + diachi)=0;return false;}
      
  }
  return false;
}


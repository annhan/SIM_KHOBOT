
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
  Serial.println(dodai);
  String datareb=String(data[18])+String(data[19]);
  TSR_ID=hexToDec(datareb);
  if (TSR_ID==36){
                  datareb=String(data[20])+String(data[21]);
                  dau=hexToDec(datareb);
                  datareb=String(data[22])+String(data[23]);
                  sau=hexToDec(datareb);
                  SRPC_ID=dau*256+sau;
                  datareb=String(data[26])+String(data[27]);
                  so_data=hexToDec(datareb);
                  for (byte i=0;i<so_data;i++){
                    Serial.print("Lan ");
                    Serial.println(i);
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
                        Serial.println(tab_id);
                        for (int j=0;j<15;j++){
                          unsigned int data_tab=*((unsigned int*)&SensorStruct + (j*8));
                          
                          if (tab_id==data_tab){
                            *((boolean*)&cambiensosanh_struc + (j))=true;
                            *((float*)&SensorStruct + ((j*8)+6))=nhietdo;
                            *((float*)&SensorStruct + ((j*8)+7))=doam;
                                    Serial.println("Dung tab");
                                    Serial.println(*((float*)&SensorStruct + ((j*8)+6)));
                                    Serial.println(*((float*)&SensorStruct + ((j*8)+7)));
                          }
                        }
                        break;
                      }
                      default :{
                        break;
                      }      
                    }
                  }
  }
  
  /*for(int i=0;i<dodai;i=i+2){
    String datareb=String(data[i]);
    datareb=datareb+String(data[i+1]);
     *((byte*)&SensorStruct + (i/2))=hexToDec(datareb);
    //Serial.println(datareb);
    if (i<8)delay(1);
    else if (i<dodai-4){tong=tong + *((byte*)&SensorStruct + (i/2));}
    else {
      if (landau){ landau=false;checksum=checksum+((*((byte*)&SensorStruct + (i/2)))*256);}
      else {checksum=checksum+*((byte*)&SensorStruct + (i/2));}
    } 
  }
  
  for (inti=0;i<dodai;i=i+2){
    
  }
  
  byte sotab=*((byte*)&SensorStruct + 9);
  switch (sotab){
    case Nhietdo:
    {
      Serial.println("Nhiệt độ");
      break;
    }             
    case Doam:
    {
      Serial.println("Độ ẩm");
      break;
    }
    case luongpin:
    {
      Serial.println("Lượng pin");
      break;
    }
    case GioMua:
    {
      unsigned int name_tab=(*((byte*)&SensorStruct + 10)*256)+(*((byte*)&SensorStruct + 11));
      float luongmua=(float)((*((byte*)&SensorStruct + 13)*256)+(*((byte*)&SensorStruct + 14)))/30.00;
      //luongmua=((*((byte*)&SensorStruct + 13)*256)+(*((byte*)&SensorStruct + 14)))%30;
      float sucgio=(float)((*((byte*)&SensorStruct + 15)*256)+(*((byte*)&SensorStruct + 16)))/600.00;
      //sucgio=((*((byte*)&SensorStruct + 15)*256)+(*((byte*)&SensorStruct + 16)))%600;
      Serial.println("Gió mưa");
      Serial.println(name_tab);
      Serial.println(luongmua);
      Serial.println(sucgio);
      break;
    }
    default:
    {
      Serial.println("Chưa Biết");
      break;
    }
  }
  Serial.println(*((byte*)&SensorStruct + 9));
  Serial.print("Tong la :");
  Serial.print(65535-checksum);
   Serial.print(";");
  Serial.println(tong);*/
}


#include <Adafruit_BMP280.h>      // For bmp280
#include <SPI.h>                  // For W25Q32 FlashMemory


#define PedSignal 35
#define ParachuteFuse 34
#define PedEnable 33
#define LiftOffCheck 23
#define Buzzer 20
#define FlightRecorderCS 15       // For W25Q32 FlashMemory



Adafruit_BMP280 BMP280;

int timeToApogei = 8000;


int start_hight;


bool LiftOffFlag;
bool FusedTimerFlag;
bool FusedBMPFlag;
bool FusedSERPFlag;
bool FusedCommandFlag;
bool FusedTimerFLag;
bool FusedBMPFLag;
bool FusedCommandFLag;
bool ApogeiBMPFlag;
bool dispFuse1;
bool dispFuse2;
bool dispFuse3;

uint32_t LiftOffTime;
uint32_t FusedTimerTime;
uint32_t FusedBMPTime;
uint32_t FusedCommandTime;
uint32_t FusedSERPTime;
uint32_t timing1;

String data = "";   // HC12


struct RecordStruct {             // For W25Q32 FlashMemory
  uint32_t timeFromStart;
  byte State;
  uint16_t Altitude;
  float Longitude;
  float Latitude;
  byte hours;
  byte minutes;
  byte seconds;
};

RecordStruct RecordData;


int hight;
int hight_prev;
int max_hight;
int c=0; 
int ident_fly; 
int sd_shet;
bool apogei; 

int page;
int offSet;


void setup() {

  pinMode(PedSignal, INPUT);
  pinMode(ParachuteFuse, OUTPUT);
  pinMode(PedEnable, OUTPUT);
  pinMode(LiftOffCheck, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(FlightRecorderCS, OUTPUT);

  digitalWrite(ParachuteFuse, 0);

  Serial.begin(9600);
  Serial3.begin(9600);

  
/*
  EEPROM.update(10, 0);
  EEPROM.update(12, 0);
  while(1)
*/
  
  if(!BMP280.begin(0x76)){
    Serial.println("BMP_Error");
    Serial3.println("BMP_Error");
    ErrorMessage("BMP280_InitError");
  }
  else{
    start_hight = int(BMP280.readAltitude());
  }
 
  if (digitalRead(LiftOffCheck)){
    Serial.println("LiftOffCheck_Error");
    Serial3.println("LiftOffCheck_Error");
    ErrorMessage("LiftOff_Error");
  }


  
  for(int i=0; i<10; i++){
    digitalWrite(Buzzer, 1);
    delay(100);
    digitalWrite(Buzzer, 0);
    delay(100);
  }

  Serial.println("BASU initialization OK. Start:");
  Serial3.println("BASU_INIT_OK");
}



void loop(){
  
  if( (digitalRead(LiftOffCheck)) && !(LiftOffFlag) ){
    LiftOffFlag = 1;
    LiftOffTime = millis();
    Serial.print("LiftOff. Time: ");
    Serial.println(LiftOffTime);
    Serial3.println("LiftOff");
  }


  if( (LiftOffFlag) && !(FusedTimerFlag) && (millis() - LiftOffTime >= timeToApogei) ){
    FusedTimerFlag = 1;
    FusedTimerTime = millis() - LiftOffTime;
    digitalWrite(Buzzer, 1);
    digitalWrite(ParachuteFuse, 1);
    Serial.print("Parachute fused from timer. Time: ");
    Serial.println(FusedTimerTime);
//    Serial3.println("FusedT");
  }
  
  if( (LiftOffFlag) && !(FusedBMPFlag) && ( ApogeiBMPFlag ) ){
    FusedBMPFlag = 1;
    FusedBMPTime = millis() - LiftOffTime;
    digitalWrite(Buzzer, 1);
    digitalWrite(ParachuteFuse, 1);
    Serial.print("Parachute fused from BMP. Time: ");
    Serial.println(FusedBMPTime);
    Serial.println("Parachute fused from BMP.");
//    Serial3.println("FusedB");
  }


  if( ( (millis()- LiftOffTime - FusedTimerTime >= 1500) && (FusedTimerFlag) )  && !FusedTimerFLag ){
    FusedTimerFLag = 1;
    Serial.print("Parachute defused. Time: ");
    Serial.println(millis() - LiftOffTime);
    digitalWrite(Buzzer, 0);
    digitalWrite(ParachuteFuse, 0);
  }
  
  if( ( (millis() - LiftOffTime - FusedBMPTime >= 1500) && (FusedBMPFlag) ) && !FusedBMPFLag ){
    FusedBMPFLag = 1;
    Serial.print("Parachute defused. Time: ");
    Serial.println(millis() - LiftOffTime);
    digitalWrite(Buzzer, 0);
    digitalWrite(ParachuteFuse, 0);
  }
  
  if( ( (millis()- LiftOffTime - FusedCommandTime >= 1500) && (FusedCommandFlag) ) && !FusedCommandFLag ){
    FusedCommandFLag = 1;
    Serial.print("Parachute defused. Time: ");
    Serial.println(millis() - LiftOffTime);
    digitalWrite(Buzzer, 0);
    digitalWrite(ParachuteFuse, 0);
  }
  


  if(Serial3.available()>0){
    char a = Serial3.read();
    if(a != 'z' and (a != char(13) and a != char(10))){
      data = data + a;
    }                 
    else {
      if( a != char(10) ){
        Serial.print("Принято по радио: ");
        Serial.print(data);
        Serial.println(";");
      }
      
      if(data == "BTest"){
        Serial3.println("BASU_OK");
        data ="";
      }

      if(( data == "sent") && LiftOffFlag ){
        String dataString="";
        dataString += String(RecordData.timeFromStart);
        dataString += " ";
        dataString += String(RecordData.Altitude);
        Serial3.println(dataString);
        if(FusedTimerFlag && !dispFuse1){
          dispFuse1 = 1;
          dataString += " ";
          dataString += "FusedT";
          Serial3.println("FusedT");
        }
        if(FusedBMPFlag && !dispFuse2){
          dispFuse2 = 1;
          dataString += " ";
          dataString += "FusedB";
          Serial3.println("FusedB");
        }
        if(FusedCommandFlag && !dispFuse3){
          dispFuse3 = 1;
          dataString += " ";
          dataString += "FusedC";
          Serial3.println("FusedC");
        }
        Serial.print("Отправлено: ");
        Serial.println(dataString); 
        
        Serial3.println("$");
        data ="";
      }
      if( (data == "ParFuse") && LiftOffFlag ){
        if(!FusedCommandFlag){
          FusedCommandTime = millis() - LiftOffTime;
          digitalWrite(Buzzer, 1);
          digitalWrite(ParachuteFuse, 1);
          Serial.print("Parachute fused from command. Time: ");
          Serial.println(FusedCommandTime);
        }
        FusedCommandFlag = 1;
        data ="";
      }
      
      data ="";
    } 
  } 

  
  
  if ( (millis() - timing1 >= 100) && LiftOffFlag){
    timing1 = millis();
    RecordData.timeFromStart = millis() - LiftOffTime;
    RecordData.Altitude = BMP280.readAltitude() - start_hight; 
    W25Q_WriteRecordStruct(page, offSet, RecordData);
    offSet += 32;
    if ( offSet > 255){
      page++;
      offSet = 0;
    }

    Extremum_BMP();
  }
  

   
}



// Тест связи.
// Фиксация в бортовой ящик временных меток о каждом действии.
// АРМ ракеты до взлёта для контроля чеки. + ответ, что арм произведён.
// Передача данных GPS о месте старта.
// Сигнал об отрыве.
// Передача телеметрии (Min: Высота и GPS + временный метки)
// Сигнал о ракрытиии парашюта по барометру и по таймеру.

void ErrorMessage(String Error){
  // ErrorMessage("BMP280_InitError");
  if(Error == "BMP280_InitError"){
    while(1){
      for(int i=0; i<6; i++){
        digitalWrite(Buzzer, 1);
        delay(100);
        digitalWrite(Buzzer, 0);
        delay(100);
      }
      delay(1000);
    }
  }

  if(Error == "LiftOff_Error"){
    while(1){
      for(int i=0; i<10; i++){
        digitalWrite(Buzzer, 1);
        delay(100);
        digitalWrite(Buzzer, 0);
        delay(100);
      }
      delay(1000);
    }
  }

  
}





void Extremum_BMP(){
  hight = int(BMP280.readAltitude()) - start_hight ;
  if( (hight >  10) && (ident_fly < 5) ){  // проверяем высоту на увеличение
    ident_fly++;                            
  }
  if( c < 6 ){
    if ( (hight < hight_prev) && (c < 5) ){
      c++;
    }
    else {
      c=0;
    }
  }
  if( (c == 5) && (ident_fly == 5) ){  
    c=6;
    ident_fly=6;
    ApogeiBMPFlag = 1;
  }
  if( hight > max_hight ){
    max_hight = hight; 
  }
  hight_prev = hight;
}


// Функции для W25Q32 Flash Memory 

// startPage is the page number where we want to start the Read fro. It can vary between 0 to 16383 (NUM_BLOCKS*256)-1. NUM_BLOCKS = 64.
// offset is the offset in the start page. It can vary between 0 to 255, since there are 256 bytes in a page.
// size is the amount of data we want to read (in Bytes).
// rData is the pointer to the array where the received data will be stored.
void W25Q_Read (uint32_t startPage, uint8_t offset, uint32_t size, uint8_t *rData){
  uint8_t tData[4];
  uint32_t memAddr = (startPage*256) + offset;
  tData[0] = 0x03;                // enable Read
  tData[1] = (memAddr>>16)&0xFF;  // MSB of the memory Address. ">> 16" выделяет 1 байт 
  tData[2] = (memAddr>>8)&0xFF;
  tData[3] = (memAddr)&0xFF;      // LSB of the memory Address
  SPISettings mySet(100000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(mySet);
  digitalWrite(FlightRecorderCS, 0);
  SPI.transfer(tData, 4);
  for (uint32_t i=0; i<size; i++) {
    rData[i] = SPI.transfer(0);
  }
  digitalWrite(FlightRecorderCS, 1);
  SPI.endTransaction();
  delay(5);
}


void W25Q_WriteEnable(void){
  uint8_t tData = 0x06;
  SPISettings mySet(100000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(mySet);
  digitalWrite(FlightRecorderCS, 0);
  SPI.transfer(tData);
  digitalWrite(FlightRecorderCS, 1);
  SPI.endTransaction();
  delay(5);
}


void W25Q_WriteDisable(void){
  uint8_t tData = 0x04;
  SPISettings mySet(100000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(mySet);
  digitalWrite(FlightRecorderCS, 0);
  SPI.transfer(tData);
  digitalWrite(FlightRecorderCS, 1);
  SPI.endTransaction();
  delay(5);
}

// sector = 4 KB or 16 pages
void W25Q_Erase_Sector (uint16_t numsector){
  uint8_t tData[4];
  uint32_t memAddr = numsector*16*256;   // Each sector contains 16 pages * 256 bytes
  W25Q_WriteEnable();
  tData[0] = 0x20;  // Erase sector
  tData[1] = (memAddr>>16)&0xFF;  // MSB of the memory Address
  tData[2] = (memAddr>>8)&0xFF;
  tData[3] = (memAddr)&0xFF; // LSB of the memory Address
  SPISettings mySet(100000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(mySet);
  digitalWrite(FlightRecorderCS, 0);
  SPI.transfer(tData, 4);
  digitalWrite(FlightRecorderCS, 1);
  SPI.endTransaction();
  delay(500);
  W25Q_WriteDisable();
}


uint32_t bytestowrite (uint32_t size, uint16_t offset)
{
  if ((size+offset)<256) return size;
  else return 256-offset;
}


// !!! Pages or sectors were cleaned before writing data, and hence any previous data in the page was lost
// page is the start page, where the write will start from.
// offset is the offset on the first page. This can vary between 0 to 255.
// size is the size of data we want to write.
// data is the pointer to the data that we want to write.
void W25Q_Write_Page (uint32_t page, uint16_t offset, uint32_t size, uint8_t *data){
  uint8_t tData[266];
  uint32_t startPage = page;
  uint32_t endPage  = startPage + ((size+offset-1)/256);
  uint32_t numPages = endPage-startPage+1;
  // Erase the sectors
  uint16_t startSector  = startPage/16;
  uint16_t endSector  = endPage/16;
  uint16_t numSectors = endSector-startSector+1;
  for (uint16_t i=0; i<numSectors; i++){
    W25Q_Erase_Sector(startSector++);
  }
  
  uint32_t dataPosition = 0;
  // write the data
  for (uint32_t i=0; i<numPages; i++){
    uint32_t memAddr = (startPage*256)+offset;
    uint16_t bytesremaining  = bytestowrite(size, offset);
    W25Q_WriteEnable();
    uint32_t indx = 0;
    tData[0] = 0x02;  // page program
    tData[1] = (memAddr>>16)&0xFF;  // MSB of the memory Address
    tData[2] = (memAddr>>8)&0xFF;
    tData[3] = (memAddr)&0xFF; // LSB of the memory Address
    indx = 4;
    uint16_t bytestosend  = bytesremaining + indx;
    for (uint16_t i=0; i<bytesremaining; i++){
      tData[indx++] = data[i+dataPosition];
    }
    SPISettings mySet(100000, MSBFIRST, SPI_MODE0);
    SPI.beginTransaction(mySet);
    digitalWrite(FlightRecorderCS, 0);
    SPI.transfer(tData, bytestosend);
    digitalWrite(FlightRecorderCS, 1);
    SPI.endTransaction();
    startPage++;
    offset = 0;
    size = size-bytesremaining;
    dataPosition = dataPosition+bytesremaining;
    delay(5);
    W25Q_WriteDisable();
  }
}


// The W25Q_Write function will be used to update the data. It takes the following parameters
// page is the start page, where the write will start from.
// offset is the offset on the first page. This can vary between 0 to 255.
// size is the size of data we want to write.
// data is the pointer to the data that we want to write.
void W25Q_Write (uint32_t page, uint16_t offset, uint32_t size, uint8_t *data){
  uint16_t startSector  = page/16;
  uint16_t endSector  = (page + ((size+offset-1)/256))/16;
  uint16_t numSectors = endSector-startSector+1;
  uint8_t previousData[4096];
  uint32_t sectorOffset = ((page%16)*256)+offset;
  uint32_t dataindx = 0;
  for (uint16_t i=0; i<numSectors; i++){
    uint32_t startPage = startSector*16;
    W25Q_Read(startPage, 0, 4096, previousData);
    uint16_t bytesRemaining = bytestowrite(size, sectorOffset);
    for (uint16_t i=0; i<bytesRemaining; i++){
      previousData[i+sectorOffset] = data[i+dataindx];
    }
    W25Q_Write_Page(startPage, 0, 4096, previousData);
    startSector++;
    sectorOffset = 0;
    dataindx = dataindx+bytesRemaining;
    size = size-bytesRemaining;
  }
}


uint8_t W25Q_ReadByte (uint32_t page, uint16_t offset){
  uint32_t memAddr = (page*256)+offset;
  uint8_t tData[4];
  tData[0] = 0x03;                // enable Read
  tData[1] = (memAddr>>16)&0xFF;  // MSB of the memory Address. ">> 16" выделяет 1 байт 
  tData[2] = (memAddr>>8)&0xFF;
  tData[3] = (memAddr)&0xFF;      // LSB of the memory Address
  SPISettings mySet(100000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(mySet);
  digitalWrite(FlightRecorderCS, 0);
  SPI.transfer(tData, 4);
  uint8_t data = SPI.transfer(0);
  digitalWrite(FlightRecorderCS, 1);
  SPI.endTransaction();
  delay(5);
  return data;
}

void W25Q_WriteByte (uint32_t page, uint16_t offset, uint8_t data){
  uint8_t tData[5];
  uint32_t Addr = (page*256)+offset;
  tData[0] = 0x02;  // page program
  tData[1] = (Addr>>16)&0xFF;  // MSB of the memory Address
  tData[2] = (Addr>>8)&0xFF;
  tData[3] = (Addr)&0xFF; // LSB of the memory Address
  tData[4] = data;
  if (W25Q_ReadByte(page, offset) == 0xFF){
    W25Q_WriteEnable();
    digitalWrite(FlightRecorderCS, 0);
    SPI.transfer(tData, 5);
    digitalWrite(FlightRecorderCS, 1);
    delay(5);
    W25Q_WriteDisable();
  }
}


void FloatToByte(uint8_t *ftoa_bytes_temp,float float_variable){
  union{
    float a;
    uint8_t bytes[4];
  } thing;

  thing.a = float_variable;
  for (uint8_t i = 0; i < 4; i++){
    ftoa_bytes_temp[i] = thing.bytes[i];
  }
}


float ByteToFloat(uint8_t * ftoa_bytes_temp){
  union {
    float a;
    uint8_t bytes[4];
  } thing;

  for (uint8_t i = 0; i < 4; i++) {
    thing.bytes[i] = ftoa_bytes_temp[i];
  }
  float float_variable =  thing.a;
  return float_variable;
}


void W25Q_Write_Float (uint32_t page, uint16_t offset, float data){
  uint8_t tempBytes[4];
  FloatToByte(tempBytes, data);
  for(int i = 0; i < 4; i++){
    W25Q_WriteByte( page, offset+i, tempBytes[i]);
  }
}


float W25Q_Read_Float (uint32_t page, uint16_t offset){
  uint8_t rData[4];
  W25Q_Read(page, offset, 4, rData);
  return (ByteToFloat(rData));
}


void uint32ToByte(uint8_t *ftoa_bytes_temp, uint32_t uint32_variable){
  union{
    uint32_t a;
    uint8_t bytes[4];
  } thing;

  thing.a = uint32_variable;
  for (uint8_t i = 0; i < 4; i++){
    ftoa_bytes_temp[i] = thing.bytes[i];
  }
}

uint32_t ByteToUint32(uint8_t * ftoa_bytes_temp){
  union {
    uint32_t a;
    uint8_t bytes[4];
  } thing;
  for (uint8_t i = 0; i < 4; i++) {
    thing.bytes[i] = ftoa_bytes_temp[i];
  }
  uint32_t uint32_variable =  thing.a;
  return uint32_variable;
}


void W25Q_Write_Single_32B (uint32_t page, uint16_t offset, uint32_t data){
  uint8_t tempBytes[4];
  uint32ToByte(tempBytes, data);
  for(int i = 0; i < 4; i++){
    W25Q_WriteByte( page, offset+i, tempBytes[i]);
  }
}


uint32_t W25Q_Read_Single_32B (uint32_t page, uint16_t offset){
  uint8_t rData[4];
  W25Q_Read(page, offset, 4, rData);
  return (ByteToUint32(rData));
}

uint32_t W25Q_Page(uint32_t Addr){
  return Addr/256;
}

uint32_t W25Q_OffSet(uint32_t Addr){
  return Addr - W25Q_Page(Addr)*256;
}

void W25Q_WriteRecordStruct(uint32_t page, uint16_t offset, RecordStruct str){
  W25Q_Write_Single_32B(page, offset, str.timeFromStart);
  uint32_t Addr = (page*256)+offset + 4;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  W25Q_WriteByte(page, offset, str.State);
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  union{
    uint16_t a;
    uint8_t bytes[4];
  } thing;
  thing.a = str.Altitude;
  W25Q_WriteByte(page, offset, thing.bytes[0]);
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  W25Q_WriteByte(page, offset, thing.bytes[1]);
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  W25Q_Write_Float(page, offset, str.Longitude);
  Addr = (page*256)+offset + 4;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  W25Q_Write_Float(page, offset, str.Latitude);
  Addr = (page*256)+offset + 4;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  W25Q_WriteByte(page, offset, str.hours);
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  W25Q_WriteByte(page, offset, str.minutes);
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  W25Q_WriteByte(page, offset, str.seconds);
}

RecordStruct W25Q_ReadRecordStruct(uint32_t page, uint16_t offset){
  RecordStruct str;
  str.timeFromStart = W25Q_Read_Single_32B(page, offset);
  uint32_t Addr = (page*256)+offset + 4;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  str.State = W25Q_ReadByte(page, offset);
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  union{
    uint16_t a;
    uint8_t bytes[4];
  } thing;
  thing.bytes[0] = W25Q_ReadByte(page, offset);
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  thing.bytes[1] = W25Q_ReadByte(page, offset);
  str.Altitude = thing.a;
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  str.Longitude = W25Q_Read_Float(page, offset);
  Addr = (page*256)+offset + 4;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  str.Latitude = W25Q_Read_Float(page, offset);
  Addr = (page*256)+offset + 4;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  str.hours = W25Q_ReadByte(page, offset);
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  str.minutes = W25Q_ReadByte(page, offset);
  Addr = (page*256)+offset + 1;
  page = W25Q_Page(Addr);
  offset = W25Q_OffSet(Addr);
  str.seconds = W25Q_ReadByte(page, offset);
  return str;
}


void dispRecordStruct(RecordStruct str){
  Serial.print("Time: ");
  Serial.print(str.timeFromStart);
  Serial.print("; ");
  Serial.print("State: ");
  Serial.print((char)str.State);
  Serial.print("; ");
  Serial.print("Altitude: ");
  Serial.print(str.Altitude);
  Serial.print("; ");
  Serial.print("Longitude: ");
  Serial.print(str.Longitude, 6);
  Serial.print("; ");
  Serial.print("Latitude: ");
  Serial.print(str.Latitude, 6);
  Serial.print("; ");

  if (str.hours < 10) Serial.print(F("0"));
  Serial.print(str.hours);
  Serial.print(":");
  if (str.minutes < 10) Serial.print(F("0"));
  Serial.print(str.minutes);
  Serial.print(":");
  if (str.seconds < 10) Serial.print(F("0"));
  Serial.print(str.seconds);
  
  Serial.println("; ");
}

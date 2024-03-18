#include <SPI.h>

#define FlightRecorderCS 15

uint8_t data[256];
uint8_t str[] = {72 ,101 ,108 ,108 ,111 ,119 ,44 ,32 ,109 ,121 ,32 ,108 ,105 ,116 ,116 ,108 ,101 ,32 ,112 ,111 ,110 ,121 ,32};
uint32_t timeFromStart;

union fourbyte {
  uint32_t dword;
  uint8_t  bytes[4];
};

int page = 0, offSet = 0;

union fourbyte val;

void setup(){
  pinMode(FlightRecorderCS, OUTPUT);
  Serial.begin(9600);
  Serial.println("Start:");
  SPI.begin();
  
  W25Q_Write_Page(0, 0, sizeof(str), str);
  W25Q_Write_Page(1, 0, sizeof(str), str);
  W25Q_Write_Page(2, 0, sizeof(str), str);
  for(int j = 0; j < 3; j++){
    W25Q_Read (j,0, 256, data);
    for(int i = 0; i<100; i++){
      Serial.print("data[");
      Serial.print(i);
      Serial.print("]: ");
      if(data[i]<127){
        Serial.print((char)data[i]);
      }
      else{
        Serial.print('-');
      }
      Serial.println(" ;");
    } 
  }
  

}



void loop(){
  /*
  if (millis() - timeFromStart >= 500){
    timeFromStart = millis(); 
    val.dword = timeFromStart;
    W25Q_Write_Page(page, offSet, sizeof(str), str);
    offSet = offSet + 23;
    if (offSet > 240){
      page++;
      offSet = 0;
    }
  
    
    Serial.print(val.bytes[0], HEX);
    Serial.print("  ");
    Serial.print(val.bytes[1], HEX);
    Serial.print("  ");
    Serial.print(val.bytes[2], HEX);
    Serial.print("  ");
    Serial.print(val.bytes[3], HEX);
    Serial.print("  ");    
    Serial.print(val.dword, DEC);
    Serial.print("  ");
    Serial.print(page, HEX);
    Serial.print("  ");    
    Serial.print(offSet, DEC);
    Serial.print("  ");
    Serial.println(timeFromStart);
  } 
  */
}

void W25Q_Reset(void){
  uint8_t tData[2];
  tData[0] = 0x66;  // Comand: "Enable Reset"
  tData[1] = 0x99;  // Comand: "Reset"
  SPISettings mySet(100000, MSBFIRST, SPI_MODE0);
  SPI.beginTransaction(mySet);
  digitalWrite(FlightRecorderCS, 0);
  SPI.transfer(tData, 2);
  digitalWrite(FlightRecorderCS, 1);
  SPI.endTransaction();
  delay(200);
}



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

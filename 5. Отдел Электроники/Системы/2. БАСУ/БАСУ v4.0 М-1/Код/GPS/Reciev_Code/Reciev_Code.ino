#include <SoftwareSerial.h>

SoftwareSerial HC12_Serial(7, 8); // RX, TX

#define ASCII_CONVERT 0
byte DataBuffer[11];

uint32_t timing;

struct GPS_Data_type{ 
  float Longitude;
  float Latitude;
  byte hours;
  byte minutes;
  byte seconds;
};

GPS_Data_type GPS_Data;
bool fl;
float f;

void setup() {
  Serial.begin(9600);
  HC12_Serial.begin(9600);
  Serial.println("Start reciev: ");
}

void loop() {
  if (millis() - timing >= 1000){
    timing = millis();
  }
  /*
  if (parsing(DataBuffer)) {
    uint8_t bytes[4];
    for(int i=0; i<4; i++){
      bytes[i] = DataBuffer[i];
    }
    GPS_Data.Longitude = ByteToFloat(bytes);
    for(int i=0; i<4; i++){
      bytes[i] = DataBuffer[i+4];
    }
    GPS_Data.Latitude = ByteToFloat(bytes);
    GPS_Data.hours = DataBuffer[8];
    GPS_Data.minutes = DataBuffer[9];
    GPS_Data.seconds = DataBuffer[10];
    dispRecordStruct(GPS_Data);
  }
  */
  parsGPS_struct(GPS_Data, DataBuffer);
  
  
}


void dispRecordStruct(GPS_Data_type str){
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


int parsing(uint8_t *buffer) {
  static bool parseStart = false;
  static byte counter = 0;
  if (HC12_Serial.available()) {
    char in = HC12_Serial.read();    
    if (in == '\n' || in == '\r') return 0; // игнорируем перевод строки    
    if (in == ';') {        // завершение пакета
      parseStart = false;
      return counter;
    }    
    if (in == '$') {        // начало пакета
      parseStart = true;
      counter = 0;
      return 0;
    }    
    if (parseStart) {       // чтение пакета
      // - '0' это перевод в число (если отправитель print)
      buffer[counter] = in - 0;
      counter++;
    }
  }
  return 0;
}

void parsGPS_struct(GPS_Data_type Strct ,uint8_t *buf){
  if (parsing(buf)) {
    uint8_t bytes[4];
    for(int i=0; i<4; i++){
      bytes[i] = buf[i];
    }
    Strct.Longitude = ByteToFloat(bytes);
    for(int i=0; i<4; i++){
      bytes[i] = buf[i+4];
    }
    Strct.Latitude = ByteToFloat(bytes);
    Strct.hours = buf[8];
    Strct.minutes = buf[9];
    Strct.seconds = buf[10];
    dispRecordStruct(Strct);
  }
}

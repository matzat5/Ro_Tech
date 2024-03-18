#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#define Buzzer 2
#define HC12_Set 7
#define GPS_Led 8

SoftwareSerial GPS_Serial(4, 3);
SoftwareSerial HC12_Serial(6, 5);
TinyGPSPlus GPS_NEO6M;

uint32_t timing;

struct GPS_Data_type{
  float Longitude;
  float Latitude;
  byte hours;
  byte minutes;
  byte seconds;
};

GPS_Data_type GPS_Data;

String data = "";

void setup() {
  pinMode(Buzzer, OUTPUT);
  pinMode(HC12_Set, OUTPUT);
  pinMode(GPS_Led, OUTPUT);
  digitalWrite(HC12_Set, 1);
  
  Serial.begin(9600);
  HC12_Serial.begin(9600);
  GPS_Serial.begin(9600);
  
  for(int i=0; i<10; i++){
    digitalWrite(Buzzer,1);
    digitalWrite(GPS_Led,1);
    delay(100);
    digitalWrite(Buzzer,0);
    digitalWrite(GPS_Led,0);
    delay(100);
  }
  Serial.println("Start:");
}

void loop() {
/*
  GPS_Serial.listen(); 
  while(GPS_Serial.available() > 0){
    GPS_NEO6M.encode(GPS_Serial.read());
  }
*/
  
  
  HC12_Serial.listen();
  
  if(HC12_Serial.available()>0){
    char a = HC12_Serial.read();
    if(a != 'z' and (a != char(13) and a != char(10))){
      data = data + a;
//      Serial.println(data);
    }
    else {
      if(data == "GPS"){

        /*
        GPS_Data.Longitude = GPS_NEO6M.location.lng();
        GPS_Data.Latitude = GPS_NEO6M.location.lat();
        GPS_Data.hours = GPS_NEO6M.time.hour();
        GPS_Data.minutes = GPS_NEO6M.time.minute();
        GPS_Data.seconds = GPS_NEO6M.time.second();
        */
        
        GPS_Data.Longitude = 23.124124;
        GPS_Data.Latitude = 12.124124;
        GPS_Data.hours = 2;
        GPS_Data.minutes = 12;
        GPS_Data.seconds = 41;
        
        HC12_Serial.listen();
        
        HC12_Serial.write('$');
        
        uint8_t bytes[4];
        FloatToByte(bytes, GPS_Data.Longitude);
        for(int i = 0; i < 4; i++){
          HC12_Serial.write(bytes[i]);
        }
        
        FloatToByte(bytes, GPS_Data.Latitude);
        for(int i = 0; i < 4; i++){
          HC12_Serial.write(bytes[i]);
        }
        
        HC12_Serial.write(GPS_Data.hours);
        HC12_Serial.write(GPS_Data.minutes);
        HC12_Serial.write(GPS_Data.seconds);
        
        HC12_Serial.write(';');
        
        if (GPS_NEO6M.location.isUpdated()){
          digitalWrite(GPS_Led,1);
        }
        else{
          digitalWrite(GPS_Led,0);
        }
        
        data ="";
        
        }
      data ="";
    }
  }  
  


  

/*  
  if ( (millis() - timing >= 1000) ){
    timing = millis();    
  }
  */
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

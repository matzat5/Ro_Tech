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



void setup() {
  pinMode(Buzzer, OUTPUT);
  pinMode(HC12_Set, OUTPUT);
  pinMode(GPS_Led, OUTPUT);
  digitalWrite(HC12_Set, 1);
  
  Serial.begin(9600);
  HC12_Serial.begin(9600);
  GPS_Serial.begin(9600);
  
  for(int i=0; i<4; i++){
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
  GPS_Serial.listen(); 
  while(GPS_Serial.available() > 0){
    GPS_NEO6M.encode(GPS_Serial.read());
  }

  if (millis() - timing >= 1000){
    timing = millis(); 
    
    HC12_Serial.listen();
    /*
    HC12_Serial.print("Time: ");
    HC12_Serial.print(timing/1000);
    
    HC12_Serial.print("; ");
    */
     
    float latitude = GPS_NEO6M.location.lat();
    HC12_Serial.print("Lati: ");
    HC12_Serial.print(latitude, 6);
    HC12_Serial.print("; ");
    HC12_Serial.print("Long: ");
    HC12_Serial.print(GPS_NEO6M.location.lng(), 6);
    HC12_Serial.print("; ");
    
    if (GPS_NEO6M.time.hour() < 10) HC12_Serial.print(F("0"));
    HC12_Serial.print(GPS_NEO6M.time.hour());
    HC12_Serial.print(":");
    if (GPS_NEO6M.time.minute() < 10) HC12_Serial.print(F("0"));
    HC12_Serial.print(GPS_NEO6M.time.minute());
    HC12_Serial.print(":");
    if (GPS_NEO6M.time.second() < 10) HC12_Serial.print(F("0"));
    HC12_Serial.print(GPS_NEO6M.time.second());
    HC12_Serial.println("; ");

    if (GPS_NEO6M.location.isUpdated()){
      digitalWrite(GPS_Led,1);
    }
    else{
      digitalWrite(GPS_Led,0);
    }
    
  }
}

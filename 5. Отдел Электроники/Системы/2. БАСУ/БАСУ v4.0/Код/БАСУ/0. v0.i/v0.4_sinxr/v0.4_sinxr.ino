#include <Adafruit_BMP280.h>      // For bmp280
#include <SPI.h>                  // For W25Q32 FlashMemory

#define PedSignal 35
#define ParachuteFuse 34
#define PedEnable 33
#define LiftOffCheck 23
#define Buzzer 20
#define FlightRecorderCS 15       // For W25Q32 FlashMemory


Adafruit_BMP280 BMP280;

String data = "";

uint32_t timing;
int Starttiming;
bool sinxrFlag = 0;
bool fl;


void setup() {
  pinMode(PedSignal, INPUT);
  pinMode(13, OUTPUT);
  pinMode(ParachuteFuse, OUTPUT);
  pinMode(PedEnable, OUTPUT);
  pinMode(LiftOffCheck, INPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(FlightRecorderCS, OUTPUT);
  
  

  Serial.begin(9600);
  Serial3.begin(9600);

  
  for(int i=0; i<10; i++){
    digitalWrite(Buzzer, 1);
    delay(100);
    digitalWrite(Buzzer, 0);
    delay(100);
  }
  
}



void loop(){
  if (Starttiming == 0){
    Starttiming = millis();
    Serial.println(Starttiming);
  }
  if (Serial3.available()) {
    char in = Serial3.read();
    if(in == '#'){
      uint8_t bytes[2];
      uint16ToByte(bytes, Starttiming);
      Serial3.write('$');
      for(int i = 0; i < 2; i++){
        Serial3.write(bytes[i]);
      }
      Serial3.write(';');
    }
  }
  
  
  if (millis() - timing > 1000){ 
    timing = millis(); 
    fl = !fl;
    digitalWrite(13, fl);
  }
}

void uint16ToByte(uint8_t *ftoa_bytes_temp, uint16_t uint32_variable){
  union{
    uint16_t a;
    uint8_t bytes[2];
  } thing;

  thing.a = uint32_variable;
  for (uint8_t i = 0; i < 2; i++){
    ftoa_bytes_temp[i] = thing.bytes[i];
  }
}

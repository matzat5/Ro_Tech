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
  char commands;
  float Longitude;
  float Latitude;
  byte hours;
  byte minutes;
  byte seconds;
};



GPS_Data_type GPS_Data;




String data = "";
String _response = "";




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
  HC12_Serial.listen();
  do {
    _response = sendATCommand("AT", true);
  } while (_response.indexOf("OK") == -1);     // Не пускать дальше, пока модем не вернет ОК
  Serial.println("HC-12 is OK. Start:");

  // заглушка GPS
  GPS_Data.commands = 'n';
  GPS_Data.Longitude = 11.111111;
  GPS_Data.Latitude = 11.111111;
  GPS_Data.hours = 11;
  GPS_Data.minutes = 11;
  GPS_Data.seconds = 11;
  HC12_Serial.listen();
}

void loop() {
   sendATCommand("AT+C010", 0);
   Serial.print('c');
   delay(100);
   
  
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


String sendATCommand(String cmd, bool waiting) {
  String _resp = "";                          // Переменная для хранения результата
  digitalWrite(HC12_Set, LOW);                     // устанвливаем в режим команды
  delay(100);                                 // ждем пока "устаканится"
  HC12_Serial.println(cmd);                          // Отправляем команду модулю
  if (waiting) {                              // Если необходимо дождаться ответа...
    _resp = waitResponse();                   // ... ждем, когда будет передан ответ
  }
  digitalWrite(HC12_Set, HIGH);                    // устанвливаем в режим работы
  delay(100);                                 // ждем пока "устаканится"
  return _resp;                               // Возвращаем результат. Пусто, если проблема
}

String waitResponse() {                       // Функция ожидания ответа и возврата полученного результата
  String _resp = "";                          // Переменная для хранения результата
  long _timeout = millis() + 30;            // Переменная для отслеживания таймаута (2 секунды)
  while (!HC12_Serial.available() && millis() < _timeout)  {}; // Ждем ответа 10 секунд, если пришел ответ или наступил таймаут, то...
  if (HC12_Serial.available()) {                     // Если есть, что считывать...
    _resp = HC12_Serial.readString();                // ... считываем и запоминаем
  }
  else {                                      // Если пришел таймаут, то...
    _resp = "ERROR";
    //    Serial.println("Timeout...");       // ... оповещаем об этом и...
  }
  return _resp;                               // ... возвращаем результат. Пусто, если проблема
}


void dispGPS_Struct(GPS_Data_type str){
  Serial.print("Command: ");
  if(str.commands == 'a'){
    Serial.print("bTest");
  }
  if(str.commands == 'b'){
    Serial.print("b");
  }
  if(str.commands == 'c'){
    Serial.print("c");
  }
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

#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#define Buzzer 2
#define HC12_Set 7
#define GPS_Led 8

SoftwareSerial GPS_Serial(4, 3);
SoftwareSerial HC12_Serial(6, 5);
TinyGPSPlus GPS_NEO6M;

uint32_t timingGlob;


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
int curChannel = 1;

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
    _response = sendATCommand("AT+С010", true);
  } while (_response.indexOf("OK") == -1);     // Не пускать дальше, пока модем не вернет ОК
  Serial.println("HC-12 is OK. Start:");
  //sendATCommand("AT+С001", 0);

  // заглушка GPS
  GPS_Data.commands = 'n';
  GPS_Data.Longitude = 11.111111;
  GPS_Data.Latitude = 11.111111;
  GPS_Data.hours = 11;
  GPS_Data.minutes = 11;
  GPS_Data.seconds = 11;
        
}

void loop() {

  
  if (millis() - timingGlob >= 500){
    timingGlob = millis();
    GPS_Serial.listen(); 
    
    Serial.println(millis());
    
    while(millis() - timingGlob < 20){        // Получение координат GPS
      while(GPS_Serial.available() > 0){
        GPS_NEO6M.encode(GPS_Serial.read());    // На будущее добавить проверку данных на валидность и обновлениеч
      }
    }
    
    while(millis() - timingGlob < 20 + 10){} // 30
      
    while(millis() - timingGlob < 30 + 60){  // 90   Получение команд от НАСУ
      HC12_Serial.listen();
      while(HC12_Serial.available()>0){
        char a = HC12_Serial.read();
        if(a != 'z' and (a != char(13) and a != char(10))){
          data = data + a;
        }
        else {
          if(data == "bTest"){
            Serial.println("bTest");
            GPS_Data.commands = 'a';
            data ="";
          }
          if(data == "bCom1"){
            Serial.println("bCom1");
            GPS_Data.commands = 'b';
            data ="";
          }
          if(data == "bCom2"){
            Serial.println("bCom2");
            GPS_Data.commands = 'c';
            data ="";
          }
          data ="";
        }
      }
    }
    
 //   sendATCommand("AT+С010", 0);

    if(millis() - timingGlob < 190 + 60 ){ // 250 Пересылание GPS и команды на БАСУ
      HC12_Serial.listen();

      HC12_Serial.write('#');
      
      HC12_Serial.write('$');

      HC12_Serial.write(GPS_Data.commands);
      
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
    }

    while(millis() - timingGlob < 250 + 60){ // 310 Получение структуры для отправки на НАСУ

    }

 //   sendATCommand("AT+С001", 0);

    while(millis() - timingGlob < 410 + 60){ // 470 Отправка структуры на НАСУ

    }

    Serial.println();
  }
  /*
  sendATCommand("AT+С010", 0);
  
  HC12_Serial.listen();
  while(HC12_Serial.available()>0){
    char a = HC12_Serial.read();
    if(a != 'z' and (a != char(13) and a != char(10))){
      data = data + a;
    }
    else {
      if(data == "GPS"){
      */
        /*
        GPS_Data.Longitude = GPS_NEO6M.location.lng();
        GPS_Data.Latitude = GPS_NEO6M.location.lat();
        GPS_Data.hours = GPS_NEO6M.time.hour();
        GPS_Data.minutes = GPS_NEO6M.time.minute();
        GPS_Data.seconds = GPS_NEO6M.time.second();
        */
      /*  
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
                
        data ="";
        
        }
      data ="";
    }
  }
//  sendATCommand("AT+С001", 0);

  if (GPS_NEO6M.location.isUpdated()){
    digitalWrite(GPS_Led,1);
  }
  else{
    digitalWrite(GPS_Led,0);
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


String sendATCommand(String cmd, bool waiting) {
  String _resp = "";                          // Переменная для хранения результата
  digitalWrite(HC12_Set, LOW);                     // устанвливаем в режим команды
  delay(60);                                 // ждем пока "устаканится"
  HC12_Serial.println(cmd);                          // Отправляем команду модулю
  if (waiting) {                              // Если необходимо дождаться ответа...
    _resp = waitResponse();                   // ... ждем, когда будет передан ответ
  }
  digitalWrite(HC12_Set, HIGH);                    // устанвливаем в режим работы
  delay(60);                                 // ждем пока "устаканится"
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

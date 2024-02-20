#include <SoftwareSerial.h>              // Подключаем библиотеку программного последовательного порта

#define TX_1 3                           // Определяем вывод TX (RX на модуле)
#define RX_1 4                           // Определяем вывод RX (TX на модуле)
#define Led_DA 5

#define TX_2 6
#define RX_2 7
#define Led_SA 8

SoftwareSerial HC_12_DA (RX_1,TX_1);      // Directional antenna
SoftwareSerial HC_12_SA (RX_2,TX_2);      // Directional antenna


#define ASCII_CONVERT '0'
unsigned long timer_1;

byte buffer_DA[5];
byte buffer_SA[5];
bool LedStat_DA = false;
bool LedStat_SA = false;


void setup() {
  Serial.begin(9600);
  HC_12_DA.begin(9600);
  HC_12_SA.begin(9600);
  pinMode(Led_DA, OUTPUT);
  pinMode(Led_SA, OUTPUT);
}                                     

void loop() {   
  
  if (parsing_DA()) {
    switch (buffer_DA[0]) {  
      case 0:
        digitalWrite(Led_DA, 0);
        break;
      case 1:
        digitalWrite(Led_DA, 1);
        break;
    }
  }
/*
  if (parsing_SA()) {
    switch (buffer_SA[1]) {  
      case 0:
        digitalWrite(Led_SA, 0);
        break;
      case 1:
        digitalWrite(Led_SA, 1);
        break;
    }
  }
*/
}


int parsing_DA() {
  static bool parseStart = false;
  static byte counter = 0;
  
  HC_12_DA.listen();
  if (HC_12_DA.available()) {
    char in = HC_12_DA.read();    
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
      buffer_DA[counter] = in - ASCII_CONVERT;
      counter++;
    }
  }
  
  return 0;
}

int parsing_SA() {
  static bool parseStart = false;
  static byte counter = 0;
  
  HC_12_SA.listen();
  if (HC_12_SA.available()) {
    char in = HC_12_SA.read();    
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
      buffer_SA[counter] = in - ASCII_CONVERT;
      counter++;
    }
  }
  return 0;
}
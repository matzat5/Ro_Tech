#include <SoftwareSerial.h>              // Подключаем библиотеку программного последовательного порта

#define TX_1 7                           // Определяем вывод TX (RX на модуле)
#define RX_1 8                           // Определяем вывод RX (TX на модуле)

SoftwareSerial HC_12 (RX_1,TX_1);      // Directional antenna

unsigned long timer_1;
bool LedStat = false;

void setup() {
  HC_12.begin(9600);
}                                     

void loop() {   
  if(millis() - timer_1 > 500){
    timer_1 = millis();
    if(LedStat){
      HC_12.write("$01;");
      LedStat = !LedStat;
    }
    else{
      HC_12.write("$10;");
       LedStat = !LedStat;
    }  
  }
}
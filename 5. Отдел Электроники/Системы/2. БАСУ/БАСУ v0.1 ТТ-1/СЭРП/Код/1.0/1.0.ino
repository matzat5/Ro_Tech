#include <Adafruit_BMP085.h>

#define Sapal PB3                                               
#define Button PB1  
#define Signal PB4  //ОТМЕТИТЬ ПИН СИГНАЛЬНОГО ДИОДА
#define TodayPressure 101500  
                                
float s=0;                                                  
float Max=0;                                             
boolean F= 0;   
                                              
Adafruit_BMP085 bmp;                                          

void setup() { 
  pinMode(Sapal, OUTPUT);      
  pinMode(Signal, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  digitalWrite(Sapal, LOW);                                                           
  if (!bmp.begin()) {
      while(true){
        digitalWrite(Signal, HIGH);     //индикация корректной работы барометра
        delay(500);
        digitalWrite(Signal, LOW);  
        delay(500); 
       }                                        
  }                                
  
}

void loop() {
  if ( (not(digitalRead(Button)))&& (not(F)) ){  //по нажатию кнопки
      for( int i=0; i<3; i++){
        digitalWrite(Signal, HIGH);    //индикация нажатия кнопки
        delay(200);
        digitalWrite(Signal, LOW);
        delay(200);
      }
      s=0;
      Max=0;   //сброс параметров
      F=1;     //флаг начала работы на 1
      digitalWrite(Sapal, LOW);
  }
                               
  if (F){
      s=0;
      for(int i=0; i<5;i++){
        s=s+float(bmp.readAltitude(TodayPressure));
      }
      s=s/5;
      if(Max<s) Max=s;                         //фиксируем макс. значение высоты
      if(Max-s>2.0){
        digitalWrite(Sapal, HIGH);  
        digitalWrite(Signal, HIGH);
        delay(3000);
        digitalWrite(Sapal, LOW);
        while(true){}                   
      }
  }
}

#include <Adafruit_BMP085.h>
#define led 4
#define Sapal 3                                               //пин запала
#define Button 1
#define Signal 4 
#define TodayPressure 101500                                  //атмосферное давление сегодня на уровне моря
/*
float alt=0.0;
float alt_start=0.0;
float alt_prev=0.0;
float millisPrevious=0.0;
float start_h=0.0;
float time_start=0.0;
int millis_b =0;
int k_on = 0;
int c=0; 
int ident_fly=0; 
int apogei=0; 
float max_alt=0.0;
boolean F = 0;                                                //флаг нажатия на кнопку
*/
Adafruit_BMP085 bmp;                                          //создание объекта барометра

void setup() {
  pinMode(led, OUTPUT); 

  if (!bmp.begin()) {
    while(true){
      digitalWrite(led, HIGH); 
      delay(200);
      digitalWrite(led, LOW);
      delay(200);   
      }
  }
  pinMode(Sapal, OUTPUT);
  pinMode(Button, INPUT_PULLUP);                                //инициализируем пин кнопки и подтягиваем землю
  digitalWrite(Sapal, LOW); 
}

void loop() {

if (not(digitalRead(Button))){
 digitalWrite(led, HIGH); 
 delay(1000);
 digitalWrite(led, LOW);
 delay(1000);   
//  F=1;
//  start_h=bmp.readAltitude(TodayPressure);
  }         
/*
if (F){                                                      //если во флаге кнопка нажата
       alt = bmp.readAltitude(TodayPressure);
    if((alt>(alt_start+5))&&(ident_fly<5))  
    ident_fly++; 
  if((c==5)&&(ident_fly==5)&&(apogei==0)) { 
    apogei=1;
    c=6;
    ident_fly=6;
    digitalWrite(Sapal, HIGH);
    digitalWrite(Signal, HIGH);
    
    while(true){
      digitalWrite(led, HIGH); 
      delay(200);
      digitalWrite(led, LOW);
      delay(200);   
    }
    
  }
  else if((alt<alt_prev)&&(c<5)){
    c=c+1;  
  } else {
    c=0;  
  }  
  if(alt>max_alt) max_alt=alt; 
  alt_prev=alt;                                          
  }
*/
}
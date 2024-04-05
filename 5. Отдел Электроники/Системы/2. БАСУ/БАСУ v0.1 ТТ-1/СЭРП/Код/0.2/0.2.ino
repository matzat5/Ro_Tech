#include <Adafruit_BMP085.h>                           
#define LedPin PB4                                          
#define TodayPressure 101500                                
#define t 400

float Sum=0;                                                
float SumP=0;                                                 
int fall =0;                                             
boolean F= 0;                                                 
Adafruit_BMP085 bmp;                                        



void setup() {
  pinMode(LedPin, OUTPUT);    
  if (!bmp.begin()){
    for(int i=0; i<4; i++){
      digitalWrite(LedPin, 0);
      delay(t);
      digitalWrite(LedPin, 1);
      delay(t);
    }  
    while(1){}
  }
  digitalWrite(LedPin, LOW);                                     
}




void loop() {                           
  if (F){                                                                                              
   SumP=Sum;                                                  
   Sum=0;                                                     
   for(int i=1;i<5; i++){                                    
    Sum=Sum+float(bmp.readAltitude(TodayPressure));            
    }
   Sum = Sum/5;                                               
   if (abs(Sum-SumP)>1.4) fall = fall+1;
   else fall= 0;
   if (fall<5) Sum= SumP;   
   if ((Sum<SumP)){
    digitalWrite(LedPin, HIGH);   
   }              
  }
}

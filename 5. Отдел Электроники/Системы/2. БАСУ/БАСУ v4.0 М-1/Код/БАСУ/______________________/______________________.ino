#include <Adafruit_BMP280.h>      // For bmp280

Adafruit_BMP280 BMP280;
#define Buzzer 20

int start_hight;

int hight;
int hight_prev;
int c=0; // количество замеров, говорящих о спуске, условимся,что 5 измерений на спуск достаточно
int ident_fly; // количество измерений, которые свидетельствуют о взлете. Условимся, что 5 достаточно.
bool apogei; // флаг, отвечает за активацию парашютной системы. Поднимается ровно 1 раз, после достижения апогея и его фиксации.
int max_hight; // максимальная высота полета


void setup() {
  pinMode(Buzzer, OUTPUT);
  Serial.begin(9600);
  if(!BMP280.begin(0x76)){
    Serial.println("BMP_Error");
  }
  else{
    start_hight = int(BMP280.readAltitude());

  }
  for(int i=0; i<10; i++){
    digitalWrite(Buzzer, 1);
    delay(100);
    digitalWrite(Buzzer, 0);
    delay(100);
  }
  Serial.println("Start:");
}

void loop() {
  Extremum_BMP();
  delay(100);
}


void Extremum_BMP(){
  hight = int(BMP280.readAltitude()) - start_hight ;
  if( (hight >  10) && (ident_fly < 5) ){  // проверяем высоту на увеличение
    ident_fly++;                            
  }
  if( c < 6 ){
    if ( (hight < hight_prev) && (c < 5) ){
      c++;
    }
    else {
      c=0;
    }
  }
  if( (c == 5) && (ident_fly == 5) ){  
    c=6;
    ident_fly=6;
    digitalWrite(Buzzer, 1);
  }
  if( hight > max_hight ){
    max_hight = hight; 
  }
  hight_prev = hight;
}

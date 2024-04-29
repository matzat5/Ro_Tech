#include <GyverHX711.h>
#include <SoftwareSerial.h>

GyverHX711 Tenza1(A2, A3, HX_GAIN64_A);
GyverHX711 Tenza2(A0, A1, HX_GAIN64_A);

SoftwareSerial MySerial(3, 4); // RX TX

#define tenza1_tare_one_bar 550000
#define tenza2_tare_one_bar 43000

int countBar_1, countBar_2;

void setup() {
  Serial.begin(9600);
  MySerial.begin(9600);
  delay(1000);
  Tenza1.tare(); 
  Tenza2.tare();
  delay(1000);
}

void loop() {
  countBar_1 = abs(Tenza1.read() / tenza1_tare_one_bar);
  countBar_2 = abs(Tenza2.read() / tenza2_tare_one_bar);

  Serial.print(countBar_1);
  Serial.print(" (");
  Serial.print(Tenza1.read());
  Serial.print("), ");
  Serial.print(countBar_2);
  Serial.print(" (");
  Serial.print(Tenza2.read());
  Serial.println(")");


  
  MySerial.print(countBar_1);
  MySerial.print(",");
  MySerial.print(countBar_2);
  MySerial.println();
  
  delay(500);
}

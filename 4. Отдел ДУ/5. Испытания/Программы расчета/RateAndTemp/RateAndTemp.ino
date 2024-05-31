#include <GyverMAX6675.h>
#include <GyverMAX6675_SPI.h>

#include <GyverMAX6675.h>
#include <Arduino.h>

// Пины модуля MAX6675K
#define CLK_PIN   13  // Пин SCK
#define DATA_PIN  12  // Пин SO
#define CS_PIN    10  // Пин CS

GyverMAX6675<CLK_PIN, DATA_PIN, CS_PIN> sens;

const int flowSensorPin = 2; 
volatile int flowPulseCount; 
float flowRate; 
unsigned int flowMilliLiters; 
unsigned long totalMilliLiters; 

void pulseCounter()
{
  flowPulseCount++;
}

void setup() {
  Serial.begin(9600);
  pinMode(flowSensorPin, INPUT);
  digitalWrite(flowSensorPin, HIGH);
  attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING);
}

void loop() {
  Serial.print(millis());
  Serial.print(';');
  if (sens.readTemp()) {            
     
    Serial.println(sens.getTemp());  
 
  } else Serial.println("Error"); 

  if (millis() - totalMilliLiters > 1000)
  {
    // Расчет расхода воды
    detachInterrupt(digitalPinToInterrupt(flowSensorPin));
    flowRate = (1000.0 / (millis() - totalMilliLiters)) * flowPulseCount;
    totalMilliLiters = millis();
    flowMilliLiters += (flowPulseCount / 7.5); 

    flowPulseCount = 0; 
    attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING); 
  }
  delay(250);    
}                 

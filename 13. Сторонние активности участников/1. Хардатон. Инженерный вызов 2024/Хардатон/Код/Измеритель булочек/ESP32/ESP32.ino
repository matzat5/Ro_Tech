#include <SoftwareSerial.h>

SoftwareSerial SerialT(13, 12);

void setup() {
  Serial.begin(9600);
  SerialT.begin(9600);
  Serial.println("Goodnight moon!");
  delay(500);
}

void loop()
{
  if (SerialT.available())
    Serial.write(SerialT.read());
  if (Serial.available())
    SerialT.write(Serial.read());
}

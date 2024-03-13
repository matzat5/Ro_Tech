#define LedPin PB4 
#define t 1000
void setup() {
  pinMode(LedPin, OUTPUT);
  for(int i=0; i<=3; i++){
    digitalWrite(LedPin, 1);
    delay(t);
    digitalWrite(LedPin, 0);
    delay(t);
  }
}

void loop() {


}

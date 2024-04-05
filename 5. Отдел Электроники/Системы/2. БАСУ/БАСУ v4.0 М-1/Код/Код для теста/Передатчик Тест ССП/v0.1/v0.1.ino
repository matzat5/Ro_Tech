#include <SoftwareSerial.h>
#define Buz 4
#define But 5
#define Led 13

SoftwareSerial Hc12(3, 2); // RX, TX

bool BuzFlag = 0;
bool FuseFlag = 0;
bool btnState;
bool ButFlag;
uint32_t btnTimer = 0;

void setup() {
  pinMode(Buz, OUTPUT);
  pinMode(Led, OUTPUT);
  pinMode(But, INPUT_PULLUP);
  Hc12.begin(9600);
  for(int i = 0; i<10; i++){
    digitalWrite(Buz, 1);
    digitalWrite(Led, 1);
    delay(100);
    digitalWrite(Buz, 0);
    digitalWrite(Led, 0);
    delay(100);
  }
}


void loop() {
  btnState = !digitalRead(But);
  if (btnState && !ButFlag && millis() - btnTimer > 100) {
    ButFlag = true;
    BuzFlag = !BuzFlag;
    FuseFlag = !FuseFlag;
    btnTimer = millis();
    Serial.println("press");
    if (FuseFlag){
      Hc12.println("kon1");
    }
    else{
      Hc12.println("koff1");
    }
  }
  if (!btnState && ButFlag && millis() - btnTimer > 100) {
    ButFlag = false;
    btnTimer = millis();

  }
  digitalWrite(Buz, BuzFlag);
  digitalWrite(Led, BuzFlag);
  
}

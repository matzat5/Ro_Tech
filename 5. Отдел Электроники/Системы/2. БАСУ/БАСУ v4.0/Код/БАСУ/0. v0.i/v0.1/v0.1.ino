#define Buz 20
String data = "";
bool F;

void setup() {
  pinMode(Buz, OUTPUT);
  Serial1.begin(9600);
  while(!Serial1){};
  analogWrite(Buz, 100);
  delay(100);
  analogWrite(Buz, 0);
  Serial1.println("Ну а что ещё сказать, хуй");
}

void loop() {
    if(Serial1.available()>0){
      char a = Serial1.read();
      if(a != 'z' and (a != char(13) and a != char(10))){
        data = data + a;
        Serial.println(data);
        }        
      else{
          if(data == "kon1"){
            Serial.println("открыт 1 клапан");
            analogWrite(Buz, 0);
            data ="";
            }
          if(data == "koff1"){
            Serial.println("закрыт 1 клапан");
            analogWrite(Buz, 255);
            data ="";
            }
          data ="";
          }
      }
}

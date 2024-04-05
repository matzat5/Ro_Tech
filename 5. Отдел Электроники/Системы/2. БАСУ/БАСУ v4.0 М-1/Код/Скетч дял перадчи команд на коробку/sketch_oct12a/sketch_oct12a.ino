
#define SET 12


String data = "";

void setup() {
  Serial.begin(9600);
  
  digitalWrite(SET, HIGH);
  pinMode(A0, OUTPUT);
  analogWrite(A0,255);
  pinMode(A1, OUTPUT);
  analogWrite(A1,255);
  pinMode(A2, OUTPUT);
  analogWrite(A2,255);
  pinMode(A3, OUTPUT);
  analogWrite(A3,255);
  pinMode(A4, OUTPUT);
  analogWrite(A4,255);
  pinMode(A5, OUTPUT);
  analogWrite(A5,255);
  pinMode(2, OUTPUT);
  digitalWrite(2,HIGH);
  pinMode(3, OUTPUT);
  digitalWrite(3,HIGH);

}

void loop() {
  if(Serial.available()>0){
    char a = Serial.read();
    if(a != 'z' and (a != char(13) and a != char(10))){
      data = data + a;
      Serial.println(data);
    }                 
    else {
      if(data == "kon1"){
        Serial.println("открыт 1 клапан");
        analogWrite(A0, 0);
        data ="";
        }
      if(data == "koff1"){
        Serial.println("закрыт 1 клапан");
        analogWrite(A0, 255);
        data ="";
        }

      if(data == "kon2"){
        Serial.println("открыт 2 клапан");
        analogWrite(A1, 0);
        data ="";
        }
      if(data == "koff2"){
        Serial.println("закрыт 2 клапан");
        analogWrite(A1, 255);
        data ="";
        }

      if(data == "kon3"){
        Serial.println("открыт 3 клапан");
        analogWrite(A2, 0);
        data ="";
        }
      if(data == "koff3"){
        Serial.println("закрыт 3 клапан");
        analogWrite(A2, 255);
        data ="";
        }

      if(data == "kon4"){
        Serial.println("открыт 4 клапан");
        analogWrite(A3, 0);
        data ="";
        }
      if(data == "koff4"){
        Serial.println("закрыт 4 клапан");
        analogWrite(A3, 255);
        data ="";
        }

      if(data == "kon5"){
        Serial.println("открыт 5 клапан");
        analogWrite(A4, 0);
        data ="";
        }
      if(data == "koff5"){
        Serial.println("закрыт 5 клапан");
        analogWrite(A4, 255);
        data ="";
        }


      if(data == "kon6"){
        Serial.println("открыт 6 клапан");
        analogWrite(A5, 0);
        data ="";
        }
      if(data == "koff6"){
        Serial.println("закрыт 6 клапан");
        analogWrite(A5, 255);
        data ="";
        }


      if(data == "kon7"){
        Serial.println("открыт 7 клапан");
        digitalWrite(2, LOW);
        data ="";
        }
      if(data == "koff7"){
        Serial.println("закрыт 7 клапан");
        digitalWrite(2, HIGH);
        data ="";
        }


      if(data == "kon8"){
        Serial.println("открыт 8 клапан");
        digitalWrite(3, 0);
        data ="";
        }
      if(data == "koff8"){
        Serial.println("закрыт 8 клапан");
        digitalWrite(3, 255);
        data ="";
        }
      data ="";
      }
    }

}

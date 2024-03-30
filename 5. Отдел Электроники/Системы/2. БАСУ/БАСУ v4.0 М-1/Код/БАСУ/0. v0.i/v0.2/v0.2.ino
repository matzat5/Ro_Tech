#define Buz 20
#define Fuse 34
#define Ped_Sign 35
String data = "";
bool F;

void setup() {
  pinMode(Buz, OUTPUT);
  pinMode(Fuse, OUTPUT);
  pinMode(Ped_Sign, OUTPUT);
  
  Serial3.begin(9600);
  while(!Serial3){};
  for(int i = 0; i<40; i++){
    digitalWrite(Buz, 1);
    digitalWrite(Ped_Sign, 1);
    delay(100);
    digitalWrite(Buz, 0);
    digitalWrite(Ped_Sign, 0);
    delay(100);
  }
}

void loop() {
    if(Serial3.available()>0){
      char a = Serial3.read();
      if(a != 'z' and (a != char(13) and a != char(10))){
        data = data + a;
        }        
      else{
          if(data == "kon1"){
            digitalWrite(Buz, 1);
            digitalWrite(Fuse, HIGH);
            digitalWrite(Ped_Sign, 1);
            data ="";
            }
          if(data == "koff1"){
            digitalWrite(Buz, 0);
            digitalWrite(Fuse, 0);
            digitalWrite(Ped_Sign, 0);
            data ="";
            }
          data ="";
          }
      }
}

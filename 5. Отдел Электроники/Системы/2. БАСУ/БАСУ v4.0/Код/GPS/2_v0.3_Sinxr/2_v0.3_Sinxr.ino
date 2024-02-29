#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#define Buzzer 2
#define HC12_Set 7
#define GPS_Led 8


SoftwareSerial HC12_Serial(6, 5);

uint32_t timing;
uint32_t timing1;
uint8_t buffSynxr[2];

int Starttiming;
int StartTimingBASU = 0;
int deltaTiming;

String data = "";
String _response = "";
bool SinxrFl;
bool fl;
bool flSinxr;

void setup() {
  pinMode(Buzzer, OUTPUT);
  pinMode(HC12_Set, OUTPUT);
  pinMode(GPS_Led, OUTPUT);
  digitalWrite(HC12_Set, 1);

  Serial.begin(9600);
  HC12_Serial.begin(9600);
  
  for(int i=0; i<10; i++){
    digitalWrite(Buzzer,1);
    digitalWrite(GPS_Led,1);
    delay(100);
    digitalWrite(Buzzer,0);
    digitalWrite(GPS_Led,0);
    delay(100);
  }
  Serial.println("Start:");
  HC12_Serial.listen();
  do {
    _response = sendATCommand("AT", true);
  } while (_response.indexOf("OK") == -1);     // Не пускать дальше, пока модем не вернет ОК
  Serial.println("HC-12 is OK. Start:");
  sendATCommand("AT+С010", 0);

        
}

void loop() {
  
  if (Starttiming == 0){
    Starttiming = millis();
    Serial.println(Starttiming);
  }

  if ( (millis() - timing1> 1000) && (StartTimingBASU == 0) ){ 
    timing1 = millis(); 
    HC12_Serial.write('#');
    while(millis() - timing1 <= 100){
      StartTimingBASU = getUint16_t(buffSynxr);
    }
  }
  
  if ( (! StartTimingBASU == 0) && (!SinxrFl) ){
    deltaTiming = - Starttiming + StartTimingBASU;
    SinxrFl = !SinxrFl;
    Serial.println(deltaTiming);
  }
/*
  if(!flSinxr ){
    flSinxr = !flSinxr;
    timing = millis() + deltaTiming; 
  }
  */
  if (millis() - timing   > 1000){ 
    timing = millis(); 
    fl = !fl;
    digitalWrite(GPS_Led, fl);
  }
  
}



String sendATCommand(String cmd, bool waiting) {
  String _resp = "";                          // Переменная для хранения результата
  digitalWrite(HC12_Set, LOW);                     // устанвливаем в режим команды
  delay(100);                                 // ждем пока "устаканится"
  HC12_Serial.println(cmd);                          // Отправляем команду модулю
  if (waiting) {                              // Если необходимо дождаться ответа...
    _resp = waitResponse();                   // ... ждем, когда будет передан ответ
  }
  digitalWrite(HC12_Set, HIGH);                    // устанвливаем в режим работы
  delay(100);                                 // ждем пока "устаканится"
  return _resp;                               // Возвращаем результат. Пусто, если проблема
}

String waitResponse() {                       // Функция ожидания ответа и возврата полученного результата
  String _resp = "";                          // Переменная для хранения результата
  long _timeout = millis() + 30;            // Переменная для отслеживания таймаута (2 секунды)
  while (!HC12_Serial.available() && millis() < _timeout)  {}; // Ждем ответа 10 секунд, если пришел ответ или наступил таймаут, то...
  if (HC12_Serial.available()) {                     // Если есть, что считывать...
    _resp = HC12_Serial.readString();                // ... считываем и запоминаем
  }
  else {                                      // Если пришел таймаут, то...
    _resp = "ERROR";
    //    Serial.println("Timeout...");       // ... оповещаем об этом и...
  }
  return _resp;                               // ... возвращаем результат. Пусто, если проблема
}



int parsing(uint8_t *buffer) {
  static bool parseStart = false;
  static byte counter = 0;
  if (HC12_Serial.available()) {
    char in = HC12_Serial.read();    
    if (in == '\n' || in == '\r') return 0; // игнорируем перевод строки    
    if (in == ';') {        // завершение пакета
      parseStart = false;
      return counter;
    }    
    if (in == '$') {        // начало пакета
      parseStart = true;
      counter = 0;
      return 0;
    }    
    if (parseStart) {       // чтение пакета
      // - '0' это перевод в число (если отправитель print)
      buffer[counter] = in - 0;
      counter++;
    }
  }
  return 0;
}

uint16_t ByteToUint16_t(uint8_t * ftoa_bytes_temp){
  union {
    uint16_t a;
    uint8_t bytes[2];
  } thing;

  for (uint8_t i = 0; i < 2; i++) {
    thing.bytes[i] = ftoa_bytes_temp[i];
  }
  uint16_t float_variable =  thing.a;
  return float_variable;
}


uint16_t getUint16_t(uint8_t *buf){
  uint16_t Strct;
  if (parsing(buf)) {
    uint8_t bytes[2];
    for(int i=0; i<2; i++){
      bytes[i] = buf[i];
    }
    Strct = ByteToUint16_t(bytes);
    
    return Strct;
  }
}

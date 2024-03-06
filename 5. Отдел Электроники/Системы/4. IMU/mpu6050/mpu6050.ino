#include <MPU6050.h>
#include <I2Cdev.h>
#include <GyverFilters.h>

#define CHECK_TIME 2 //период замеров
#define TO_DEG 57.2957f //для перевода в градусы


uint32_t timer = 0;  

const float G = 9.81;
MPU6050 mpu;
short ax0, ay0, az0, gx0, gy0, gz0; //ускорения и угловые скорости по осям
float angles[3]; //углы по осям
float a[3]; //переведённые ускорения из сырых значений
float g[3]; //переведённые угловые скорости из сырых значений
GKalman KFilter1(40, 40); // создала несколько для того, чтобы менять параметры каждого отдельно
GKalman KFilter0(40, 40); // когда создавала один для все, значения одной переменной почему-то начинали зависеть от значений другой переменной
GKalman KFilter2(40, 40);
GKalman KFilter3(40, 20);


void setup() {
  
  Wire.begin();
  Serial.begin(9600);
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "MPU6050 OK" : "MPU6050 FAIL");
  mpu.CalibrateAccel(6); //калибировка
  mpu.CalibrateGyro(); 
  
  for(int i = 0; i < 3; i++){
    angles[i] = 0; //обмнуление градусов по осям
  }
  //Timer1.setPeriod(200000);
  //Timer1.enableISR();
}

void loop() {
  mpu.getMotion6(&ax0, &ay0, &az0, &gx0, &gy0, &gz0);
  agRight(a, g); //преобразование сырых значений
  
  ////////////////////////////////////
  // a[0] = KFilter0.filtered(a[0]);
  // a[1] = KFilter1.filtered(a[1]);
  // a[2] = KFilter2.filtered(a[2]);

  // Serial.print("$");
  // Serial.print(a[0]); Serial.print(" ");
  // Serial.print(a[1]); Serial.print(" ");
  // Serial.print(a[2]); Serial.print(" ");
  // Serial.println(";");
  //////////////////////////////////////////
  
//////////////////////////////////  X
  // Serial.print("$");
  // Serial.print(a[0]);
  // a[0] = KFilter0.filtered(a[0]);
  // Serial.print(" ");
  // Serial.print(a[0]);
  // Serial.println(";");
////////////////////////////////// Y
  // Serial.print("$");
  // Serial.print(a[1]);
  // a[1] = KFilter1.filtered(a[1]);
  // Serial.print(" ");
  // Serial.print(a[1]);
  // Serial.println(";");
  ////////////////////////////////// Z
  // Serial.print("$");
  // Serial.print(a[2]);
  // a[2] = KFilter2.filtered(a[2]);
  // Serial.print(" ");
  // Serial.print(a[2]);
  // Serial.println(";");
  //////////////////////////////////
  // for(int i = 0; i < 3; i ++){
  //   gm[i] = KFilter3.filtered(gm[i]);
  //   angles[i] += gm[i]*CHECK_TIME/1000.0;
  // }
  // Serial.print("$");
  // Serial.print(angles[0]); Serial.print(" ");
  // Serial.print(angles[1]); Serial.print(" ");
  // Serial.print(angles[2]); Serial.print(" ");
  // Serial.println(";");
  if (millis() - timer >= CHECK_TIME){ //измерение через каждые CHECK_TIME милисекунд

    for(int i = 0; i < 3; i ++){
      g[i] = KFilter3.filtered(g[i]);  //фильрация всех угловых скоростей
    }
    if(fabs(g[0]) >= 0.05) {  //это для того, чтобы угол просто так не рос из-за маленьких шумов
      angles[0] += g[0]*(millis()-timer)*9/1000.0; //интегрирование
      //angles[0] = int(angles[0]*100)%36000/100.0;  //кратность 360 градусам
      //Serial.println(millis()-timer);
    }
    Serial.print("$");
    Serial.print(angles[0]); 
    Serial.print(" "); 
    Serial.println(g[0]);
    Serial.println(";");

    // Serial.print("$");
    // Serial.print(g[0]); 
    // Serial.print(" "); 
    // Serial.print(g[1]); 
    // Serial.print(" "); 
    // Serial.println(g[2]);
    // Serial.println(";");
    timer = millis();
  
    
  
  }
  //delay(CHECK_TIME);
}

void agRight(float arr1[], float arr2[]){
  arr1[0] = ax0/16384.0*G;
  arr1[1] = ay0/16384.0*G;
  arr1[2] = az0/16384.0*G;

  arr2[0] = gx0/131.0;
  arr2[1] = gy0/131.0;
  arr2[2] = gz0/131.0;
}

// ISR(TIMER1_A) {  // это я пыталась с помощью прерываний отсчёты времени регулировать
//   mpu.getMotion6(&ax0, &ay0, &az0, &gx0, &gy0, &gz0);
//   mpu.getRotation(&gx0, &gy0, &gz0);
//   agRight(a, g);
//   for(int i = 0; i < 3; i ++){
//       g[i] = KFilter3.filtered(g[i]);
//   }
//   if(fabs(g[0]) >= 0.05) {
//     angles[0] += g[0]*2/1000.0;
//     //angles[0] = int(angles[0]*100)%36000/100.0;
//   }
//   Serial.print(angles[0]); 
//   Serial.print(" "); 
//   Serial.println(g[0]);
// }



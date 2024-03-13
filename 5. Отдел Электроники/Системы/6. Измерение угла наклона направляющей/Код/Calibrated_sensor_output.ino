#include "FastIMU.h"
#include <Wire.h>

#define IMU_ADDRESS 0x6B    //Change to the address of the IMU
#define PERFORM_CALIBRATION //Comment to disable startup calibration
LSM6DS3 IMU;               //Change to the name of any supported IMU! 

// Currently supported IMUS: MPU9255 MPU9250 MPU6886 MPU6500 MPU6050 ICM20689 ICM20690 BMI055 BMX055 BMI160 LSM6DS3 LSM6DSL

calData calib = { 0 };  //Calibration data
AccelData accelData;    //Данные с датчиков. Магнитометр не используем - смотрите на перменные с индексом 'Accel'
GyroData gyroData;
MagData magData;

void setup() {
  Wire.begin();
  Wire.setClock(400000); //400khz clock
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  int err = IMU.init(calib, IMU_ADDRESS);     //если калибровка выдаёт ошибку
  if (err != 0) {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }
  
#ifdef PERFORM_CALIBRATION                              //калибровка магнитометра
  Serial.println("FastIMU calibration & data example");
  if (IMU.hasMagnetometer()) {
    delay(1000);
    Serial.println("Move IMU in figure 8 pattern until done.");
    delay(3000);
    IMU.calibrateMag(&calib);
    Serial.println("Magnetic calibration done!");
  }
  else {
    delay(5000);
  }

  delay(5000);                                          //калибровка акселерометра
  Serial.println("Keep IMU level.");
  delay(5000);
  IMU.calibrateAccelGyro(&calib);
  Serial.println("Calibration done!");
  Serial.println("Accel biases X/Y/Z: ");
  Serial.print(calib.accelBias[0]);
  Serial.print(", ");
  Serial.print(calib.accelBias[1]);
  Serial.print(", ");
  Serial.println(calib.accelBias[2]);
 
  if (IMU.hasMagnetometer()) {                    //вывод данных с магнитометра
    Serial.println("Mag biases X/Y/Z: ");
    Serial.print(calib.magBias[0]);
    Serial.print(", ");
    Serial.print(calib.magBias[1]);
    Serial.print(", ");
    Serial.println(calib.magBias[2]);
    Serial.println("Mag Scale X/Y/Z: ");
    Serial.print(calib.magScale[0]);
    Serial.print(", ");
    Serial.print(calib.magScale[1]);
    Serial.print(", ");
    Serial.println(calib.magScale[2]);
  }
  delay(5000);
  IMU.init(calib, IMU_ADDRESS);
#endif

  //err = IMU.setGyroRange(500);      //USE THESE TO SET THE RANGE, IF AN INVALID RANGE IS SET IT WILL RETURN -1
  //err = IMU.setAccelRange(2);       //THESE TWO SET THE GYRO RANGE TO ±500 DPS AND THE ACCELEROMETER RANGE TO ±2g
  
  if (err != 0) {
    Serial.print("Error Setting range: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }
}

void loop() {                                               //вывод данных с акселерометра
  IMU.update();
  IMU.getAccel(&accelData);
  /*Serial.print(accelData.accelX);
  Serial.print("\t");
  Serial.print(accelData.accelY);
  Serial.print("\t");
  Serial.println(accelData.accelZ);
  Serial.print("\t"); */
  
  if (IMU.hasMagnetometer()) {                     
    IMU.getMag(&magData);
    Serial.print("\t");
    Serial.print(magData.magX);
    Serial.print("\t");
    Serial.print(magData.magY);
    Serial.print("\t");
    Serial.print(magData.magZ);
  }

  Serial.print(atan((accelData.accelX)/(sqrt(sq(accelData.accelY)+ sq(accelData.accelZ))))*(180/3.14));           //вывод полученных углов
  Serial.print("***");
  Serial.print(atan((accelData.accelY)/(sqrt(sq(accelData.accelX)+ sq(accelData.accelZ))))*(180/3.14));
  Serial.print("***");
  Serial.println(atan((accelData.accelZ)/(sqrt(sq(accelData.accelX)+ sq(accelData.accelY))))*(180/3.14));

  
  delay(50);
}

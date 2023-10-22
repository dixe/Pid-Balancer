/*
from here https://lastminuteengineers.com/mpu6050-accel-gyro-arduino-tutorial/
 try filter from here https://www.youtube.com/watch?v=5HuN9iL-zxU 
 
 SCL = A5
 SDA = A4
 
*/
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

float elapsedTime, currentTime, previousTime;
float angle_est = 0.0;

Adafruit_MPU6050 mpu;

float RateCalibrationRoll = 0.035;
float RateCalibrationPitch = 0;
float RateCalibrationYaw = 0;



void gyroSetup() {
  // Try to initialize!
  //Serial.println("Begin");
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  
  //Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // TODO Find error so we can subtract it from all future readings
  //FindError();

  // init angle to acc angle
  sensors_event_t a, g, temp; 
  mpu.getEvent(&a, &g, &temp); 
 
  float accX = a.acceleration.x;
  float accY = a.acceleration.y;
  float accZ = a.acceleration.z;
  
  angle_est = atan(accY/sqrt(accX*accX+accZ*accZ))*1/(3.142/180);
  //FindError();
  currentTime = millis();
  
}


void FindError() {
  
  Serial.print("Finding error");
  
  int i = 400;
  
  float totalRoll = 0.0;
  
  while (i > 0) {
    i = i - 1;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

  
    totalRoll += g.gyro.x;
  }

  float err = totalRoll / 400;

  Serial.print("Total Roll Error: ");
  Serial.println(err);
  

}



// Return Roll angle
float gyroLoop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  float rateRoll = g.gyro.x - RateCalibrationRoll;
  float ratePitch = g.gyro.y - RateCalibrationPitch;
  float rateYaw = g.gyro.z - RateCalibrationYaw;
  

  float accX = a.acceleration.x;
  float accY = a.acceleration.y;
  float accZ = a.acceleration.z;
  
  float angleRoll = atan(accY/sqrt(accX*accX+accZ*accZ))*1/(3.142/180);
  float anglePitch = -atan(accX/sqrt(accY*accY+accZ*accZ))*1/(3.142/180);


  previousTime = currentTime;
  currentTime = millis();

  elapsedTime = currentTime  - previousTime;

  float change_gyro = (rateRoll * (elapsedTime / 1000.0)) * (180/ PI);

  float change_acc = angleRoll - angle_est;
  angle_est =  angle_est + 0.98 * change_gyro + change_acc * 0.02;

  return angle_est;
  
  /*
  kalman_1d(KalmanAngleRoll, KalmanUncertaintyAngleRoll, rateRoll, angleRoll);
  
  KalmanAngleRoll = Kalman1DOutput[0]; 
  
  KalmanUncertaintyAngleRoll = Kalman1DOutput[1];
  kalman_1d(KalmanAnglePitch, KalmanUncertaintyAnglePitch, ratePitch, anglePitch);
  
  KalmanAnglePitch=Kalman1DOutput[0]; 
  KalmanUncertaintyAnglePitch = Kalman1DOutput[1];

  
  Serial.print("Roll Angle [°] ");
  Serial.print(KalmanAngleRoll);
  Serial.print(" Pitch Angle [°] ");
  Serial.println(KalmanAnglePitch);
  */

  /* why???
  while (micros() - LoopTimer < 4000);
  LoopTimer=micros();
  */
}



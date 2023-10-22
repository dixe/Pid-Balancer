/*
from here https://lastminuteengineers.com/mpu6050-accel-gyro-arduino-tutorial/
 try filter from here https://www.youtube.com/watch?v=5HuN9iL-zxU 
*/
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

float elapsedTime, currentTime, previousTime;

Adafruit_MPU6050 mpu;


struct AngleDiff {
  float x;
  float y;
  float z;
};

struct AngleDiff diff;
void setup() {
  // setup serial baud rate
  Serial.begin(115200);

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // TODO Find error so we can subtract it from all future readings

  delay(100);
  
  currentTime = millis();
}



void loop() {
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

  // why???
  while (micros() - LoopTimer < 4000);
  LoopTimer=micros();
}



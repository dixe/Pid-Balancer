/*
from here https://lastminuteengineers.com/mpu6050-accel-gyro-arduino-tutorial/
   
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
  Serial.begin(9600);

Serial.println("Start to find MPU6050 chip");
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

  delay(100);
  
  currentTime = millis();
}


void loop() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
/*
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
*/
  
  /* Print out the values */
  /*
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  */



 
/*
 // Rotation in rad/s
  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");


// rotation in rad

  diff.x = g.gyro.x * elapsedTime;
  diff.y = g.gyro.y * elapsedTime;
  diff.z = g.gyro.z * elapsedTime;  
 Serial.print("Rotation X: ");
  Serial.print(diff.x);
  Serial.print(", Y: ");
  Serial.print(diff.y);
  Serial.print(", Z: ");
  Serial.print(diff.z);
  Serial.println(" rad");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");
*/

 // Serial plotter output
  Serial.print(a.orientation.x);
  Serial.print(",");
  Serial.print(a.orientation.y);
  Serial.print(",");
  Serial.print(a.orientation.z);
  Serial.print(", ");
  Serial.print(g.gyro.x);
  Serial.print(",");
  Serial.print(g.gyro.y);
  Serial.print(",");
  Serial.print(g.gyro.z);
  Serial.println("");
  delay(10);
}

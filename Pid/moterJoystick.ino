#include <Servo.h> 

// Joystick Y - A0
// yellow signal wire -  9 (D9)
// Red - 5v
// Brown - Ground

// joystick y axis connected to A0 only upper part is used, not touched is neutral 0, full up y is full power, down and left/right does nothing
// joystick x axis not used
// joystick key (press) not used

Servo esc;

int escPin = 9;
int yPin = A0;
int yValue = 0;
int minPulseRate = 1000;
int maxPulseRate = 2000;
int throttleChangeDelay = 100;
int maxThrottle = 90; // should be between 0 and 180, 180 is max for motor, so more than that does not make sense, night crash

void motorSetup() {  
  pinMode(yPin,INPUT);
  // Attach the the servo to the correct pin and set the pulse range
  esc.attach(escPin, minPulseRate, maxPulseRate); 
  // Write a minimum value (most ESCs require this correct startup)
  esc.write(0);
  
}


void motorLoop() {
  // middle position is 509, subtract that and  multiply value by 2 to get range of 0..1023 again
  yValue = (analogRead(yPin) - 800) * 2; 
  
  // Read the new throttle value
  int throttle = normalizeThrottle( yValue);
    
  // Change throttle to the new value
  // TODO works but is kinda jumpy, ensure more smooth changing, ie difference between now and target, but with max gradient 

  esc.write(throttle);
  //changeThrottle(throttle);
}


int readThrottle() {
  int throttle = esc.read();   
  return throttle;
}

//Assume input 0-1023 from analogInput
// Ensure the throttle value is between 0 - maxThrottle
int normalizeThrottle(int value) {
  value = ((double)value / 1023) * maxThrottle;  
  if( value < 0 )
    return 0;
  if( value > maxThrottle )
    return maxThrottle;
  return value;
}

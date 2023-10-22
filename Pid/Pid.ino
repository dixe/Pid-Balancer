
void setup() {  


  Serial.begin(9600);
  Serial.setTimeout(500);

  //Serial.println("Setting up gyro");
  gyroSetup();
  
  //Serial.println("Setting up Motor");
  motorSetup();
  
  //Serial.println("Finished setup"); 
  
  
}

float roll_cur = 0.0;
int throttle_cur = 0;

void loop() {
  roll_cur = gyroLoop();

  Serial.print("Roll: ");
  Serial.println(roll_cur);
  

  // tell moters how much 
  motorLoop();
  
}


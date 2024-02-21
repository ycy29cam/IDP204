int lineStates[4];

void forward(int speed){
  // Rotate both motors forward at the given speed
  left->setSpeed(speed);
  right->setSpeed(speed);
  left->run(FORWARD);
  right->run(FORWARD);
}

void backward(int speed){
  // Rotate both motors backward at the given speed
  left->setSpeed(speed);
  right->setSpeed(speed);
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void turnLeft(){
  // Rotate the robot to the right, for a given duration (which determines the angle rotated)
  left->setSpeed(255);
  right->setSpeed(125);
  left->run(FORWARD);
  right->run(BACKWARD);
  // Duration to be calibrated to turn the desired amount
}

void turnRight(){
  // Rotate the robot to the left, for a given duration (which determines the angle rotated)
  left->setSpeed(125);
  right->setSpeed(255);
  left->run(BACKWARD);
  right->run(FORWARD);
  // Duration to be calibrated to turn the desired amount
}

void stop(){
  left->setSpeed(0);
  right->setSpeed(0);
  left->run(RELEASE);
  right->run(RELEASE);
}

void follow(int speed_1, int speed_2){
  // Rotate both motors forward at the given speed
  left->setSpeed(speed_1);
  right->setSpeed(speed_2);
  left->run(FORWARD);
  right->run(FORWARD);
}

void readLine(){
  lineStates[0] = digitalRead(LOut);
  lineStates[1] = digitalRead(LIn);
  lineStates[2] = digitalRead(RIn);
  lineStates[3] = digitalRead(ROut);
}

void adjust(int lineStates[4]){
  // Maneuver the robot based on the states of the line sensors
  if (lineStates[1] == 0 && lineStates[2] == 0){
    forward(speed);
  }
  else if (lineStates[1] == 0 && lineStates[2] == 1){
    //follow(speed - 50, speed);
    turnRight();
  }
  else if (lineStates[1] == 1 && lineStates[2] == 0){
    //follow(speed, speed - 50);
    turnLeft();
  }
  else{
    Serial.println("Error: both inner line sensors are on the white line");
  }
  readLine();
}
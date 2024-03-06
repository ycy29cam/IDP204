Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *right = AFMS.getMotor(1);
Adafruit_DCMotor *left = AFMS.getMotor(2);
// D9 and D10 are used for the motor shield

int LOut = 2, LIn = 3, RIn = 4, ROut = 5;
// Declare pins used by the 4 line sensors
int lineStates[4];
// Declare a list to store the 4 states of 4 separate line sensors

int speed = 200;
// Declare the default speed of the motors
int nintyDegrees = 700;
// Declare the time required for the robot to turn ninty degrees using the turnLeft or turnRight functions

int leftWeightArray[10] = {0};
int rightWeightArray[10] = {0};
// Declare arrays to store past values of the line sensor states, for weighting the turn
int head = 0;
// Declare a variable to store the position in the weight arrays for the next recording
int arraySize = 0;
// Declare a variable to store the length of the weight arrays

void readLine(){
  lineStates[0] = digitalRead(LOut);
  lineStates[1] = digitalRead(LIn);
  lineStates[2] = digitalRead(RIn);
  lineStates[3] = digitalRead(ROut);
}

void forward(int speed){
  // Rotate both motors forward at the given speed
  left->setSpeed(speed);
  right->setSpeed(speed);
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void backward(int speed){
  // Rotate both motors backward at the given speed
  left->setSpeed(speed);
  right->setSpeed(speed);
  left->run(FORWARD);
  right->run(FORWARD);
}

void turnLeft(){
  // Rotate the robot to the right, for a given duration (which determines the angle rotated)
  // This is used for sharp turns, ie. approaching the block
  left->setSpeed(125);
  right->setSpeed(200);
  left->run(FORWARD);
  right->run(BACKWARD);
  // Duration to be calibrated to turn the desired amount
}

void turnRight(){
  // Rotate the robot to the left, for a given duration (which determines the angle rotated)
  // This is used for sharp turns, ie. approaching the block
  left->setSpeed(200);
  right->setSpeed(125);
  left->run(BACKWARD);
  right->run(FORWARD);
  // Duration to be calibrated to turn the desired amount
}

void arcTurnRight(){
  // This is used for gentle turns (cutting the corners), ie. ordinary junctions
  /*
  left->setSpeed(200);
  right->setSpeed(80);
  left->run(BACKWARD);
  right->run(FORWARD);
  */
  left->setSpeed(200);
  right->setSpeed(40);
  left->run(BACKWARD);
  right->run(FORWARD);
}

void arcTurnLeft(){
  // This is used for gentle turns (cutting the corners), ie. ordinary junctions
  left->setSpeed(80);
  right->setSpeed(200);
  left->run(FORWARD);
  right->run(BACKWARD);
}

void stop(){
  left->setSpeed(0);
  right->setSpeed(0);
  left->run(RELEASE);
  right->run(RELEASE);
}

void turn(int direction){
  // Combined turning functions, with delays and line detection
  // 1 indicates a right turn and 2 indicates a left turn
  if (direction == 1){
    arcTurnRight();
    delay(1200);

    readLine();
    while(lineStates[1] == 0){
    arcTurnRight();
    readLine();
    }
  }
  else if (direction == 2){
    arcTurnLeft();
    delay(1200);

    readLine();
    while(lineStates[2] == 0){
    arcTurnLeft();
    readLine();
    }
  }
}

void follow(int speed_1, int speed_2){
  // Rotate both motors forward at the given speed
  left->setSpeed(speed_1);
  right->setSpeed(speed_2);
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void recordLineValue(int lineStates[4]){
  leftWeightArray[head] = lineStates[1];
  rightWeightArray[head] = lineStates[2];
  head = (head + 1) % 10;
  if (arraySize < 10){
    arraySize++ ;
  }
}

int sumWeight(int weight[]){
  int total = 0;
  for (int i = 0; i < arraySize; i++) {
    total += weight[i];
  }
  return (total * 30 / 2) + 30;
}

void adjust(int lineStates[4]){
  recordLineValue(lineStates);
  int leftWeight = sumWeight(leftWeightArray);
  int rightWeight = sumWeight(rightWeightArray);

  // Maneuver the robot based on the states of the line sensors
  if (lineStates[1] == 0 && lineStates[2] == 0){
    forward(speed);
  }
  else if (lineStates[1] == 0 && lineStates[2] == 1){
    follow(speed, speed - rightWeight);
    //follow(speed - rightWeight, speed);
    //turnRight();
  }
  else if (lineStates[1] == 1 && lineStates[2] == 0){
    follow(speed - leftWeight, speed);
    //follow(speed, speed - rightWeight);
    //turnLeft();
  }
  else{
    Serial.println("Error: both inner line sensors are on the white line");
  }
  readLine();
}

void drive_route(int* journey, int number_of_junctions) {
   
    int journey_count = 0; 
    while (journey_count < number_of_junctions) {

        // Move forward consistently
        readLine();
        while(lineStates[0] == 0 && lineStates[3] == 0){
            adjust(lineStates);
            readLine();
        }

        stop();
        delay(250);


      Serial.print(journey_count);
      Serial.println(journey[journey_count]);
        if (journey[journey_count] == 0) {
                // robot will continue to move forwards
            Serial.println("I should move forward");
            forward(speed);
            delay(600);
        }
        else if (journey[journey_count] == 1) { 
            Serial.println("I should turn right");
            turn(1);
        }
        else if (journey[journey_count] == 2) {
          Serial.println("I should turn left");
            turn(2);
        }

        stop();
        delay(250);
        journey_count++;
    }
}
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *left = AFMS.getMotor(1);
Adafruit_DCMotor *right = AFMS.getMotor(2);
// D9 and D10 are used for the motor shield

int LOut = 2, LIn = 3, RIn = 4, ROut = 5;
// Declare pins used by the 4 line sensors
int lineStates[4];
// Declare a list to store the 4 states of 4 separate line sensors

int blue = 6; // Blue LED
int red = 7; // Red LED
int green = 8; // Green LED

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

long blink_time = millis();
int blinkState = 0;

void blinkLED(long current_time){
  if ((current_time - blink_time) > 500){
    blinkState = blinkState == 0 ? 1 : 0;
    blink_time = current_time;
  }
  digitalWrite(blue, blinkState);
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
  left->setSpeed(125);
  right->setSpeed(200);
  left->run(FORWARD);
  right->run(BACKWARD);
  // Duration to be calibrated to turn the desired amount
}

void turnRight(){
  // Rotate the robot to the left, for a given duration (which determines the angle rotated)
  left->setSpeed(200);
  right->setSpeed(125);
  left->run(BACKWARD);
  right->run(FORWARD);
  // Duration to be calibrated to turn the desired amount
}

void arcTurnRight(){
  left->setSpeed(200);
  right->setSpeed(0);
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void arcTurnLeft(){
  left->setSpeed(0);
  right->setSpeed(200);
  left->run(BACKWARD);
  right->run(BACKWARD);
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
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void readLine(){
  lineStates[0] = digitalRead(LOut);
  lineStates[1] = digitalRead(LIn);
  lineStates[2] = digitalRead(RIn);
  lineStates[3] = digitalRead(ROut);
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
  Serial.print(leftWeight);
  Serial.println(rightWeight);

  // Maneuver the robot based on the states of the line sensors
  if (lineStates[1] == 0 && lineStates[2] == 0){
    forward(speed);
  }
  else if (lineStates[1] == 0 && lineStates[2] == 1){
    follow(speed, speed - rightWeight);
    //turnRight();
  }
  else if (lineStates[1] == 1 && lineStates[2] == 0){
    follow(speed - leftWeight, speed);
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

        if (journey[journey_count] == 0) {
                // robot will continue to move forwards
            forward(250);
        }
        else if (journey[journey_count] == 1) { 
            arcTurnRight();
        }
        else if (journey[journey_count] == 2) {
            arcTurnLeft();
        }
        /*
        else if (journey[journey_count] == 3) {
            ArcTurnRight();
        }
        else if (journey[journey_count] == 4) {
            ArcTurnLeft();
        }
        */

        stop();
        delay(250);
        journey_count++;
    }
}
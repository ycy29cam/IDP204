Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *right = AFMS.getMotor(2);
Adafruit_DCMotor *left = AFMS.getMotor(1);
// Declare the motor objects

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
int routeCounter = 0;
// Declare the routeCounter which is used to iterate the routes array

int leftWeightArray[10] = {0};
int rightWeightArray[10] = {0};
// Declare arrays to store past values of the line sensor states, for weighting the turn
int head = 0;
// Declare a variable to store the position in the weight arrays for the next recording
int arraySize = 0;
// Declare a variable to store the length of the weight arrays

long blink_time = millis();
// Declare a variable to store the time, when the blue LED is toggled
int blinkState = 0;
// Declare a variable to store the current state of the blue LED, to be checked for blinking

void blinkLED(long current_time){
  if ((current_time - blink_time) > 500){
    blinkState = blinkState == 0 ? 1 : 0;
    blink_time = current_time;
  }
  digitalWrite(blue, blinkState);
  // If it has been more than 0.5s since the LED was last toggled, toggle the LED (therefore the LED blinks)
}

void readLine(){
  lineStates[0] = digitalRead(LOut);
  lineStates[1] = digitalRead(LIn);
  lineStates[2] = digitalRead(RIn);
  lineStates[3] = digitalRead(ROut);
  // Read all four line sensors
}

void forward(int speed){
  // Rotate both motors forward at the given speed
  blinkLED(millis());
  left->setSpeed(speed);
  right->setSpeed(speed);
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void backward(int speed){
  // Rotate both motors backward at the given speed
  blinkLED(millis());
  left->setSpeed(speed - 50);
  right->setSpeed(speed);
  left->run(FORWARD);
  right->run(FORWARD);
}

void turnLeft(){
  // Rotate the robot to the right, for a given duration (which determines the angle rotated)
  // This is used for sharp turns, ie. approaching the block
  blinkLED(millis());
  left->setSpeed(200);
  right->setSpeed(200);
  left->run(FORWARD);
  right->run(BACKWARD);
  // Duration to be calibrated to turn the desired amount
}

void turnRight(){
  // Rotate the robot to the left, for a given duration (which determines the angle rotated)
  // This is used for sharp turns, ie. approaching the block
  blinkLED(millis());
  left->setSpeed(200);
  right->setSpeed(200);
  left->run(BACKWARD);
  right->run(FORWARD);
  // Duration to be calibrated to turn the desired amount
}

void arcTurnRight(){
  // This is used for gentle turns (cutting the corners), ie. ordinary junctions
  blinkLED(millis());
  left->setSpeed(200);
  right->setSpeed(60); // Not symmetric
  left->run(BACKWARD);
  right->run(FORWARD);
}

void arcTurnLeft(){
  // This is used for gentle turns (cutting the corners), ie. ordinary junctions
  blinkLED(millis());
  left->setSpeed(80);
  right->setSpeed(200);
  left->run(FORWARD);
  right->run(BACKWARD);
}

void turn(int direction){
  // Combined turning functions, with delays and line detection
  // 1 indicates a right turn and 2 indicates a left turn
  if (direction == 1){
    blinkLED(millis());
    arcTurnRight();
    delay(1200);

    readLine();
    while(lineStates[1] == 0){
    blinkLED(millis());
    arcTurnRight();
    readLine();
    }
  }
  else if (direction == 2){
    blinkLED(millis());
    arcTurnLeft();
    delay(1200);

    readLine();
    while(lineStates[2] == 0){
    blinkLED(millis());
    arcTurnLeft();
    readLine();
    }
  }
}

void stop(){
  left->setSpeed(0);
  right->setSpeed(0);
  left->run(RELEASE);
  right->run(RELEASE);
  // Stop the AGV
}

void follow(int speed_1, int speed_2){
  // Rotate both motors forward at the given speed
  left->setSpeed(speed_1);
  right->setSpeed(speed_2);
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void recordLineValue(int lineStates[4]){
  // Store the past 10 values of line sensors 1 and 2
  leftWeightArray[head] = lineStates[1];
  rightWeightArray[head] = lineStates[2];
  head = (head + 1) % 10;
  if (arraySize < 10){
    arraySize++ ;
  }
}

int sumWeight(int weight[]){
  // Compute the sum of the past 10 values of line sensors 1 and 2
  int total = 0;
  for (int i = 0; i < arraySize; i++) {
    total += weight[i];
  }
  return (total * 30 / 2) + 30;
  // Adjust the turning weighting based on the past sensor readings
}

void adjust(int lineStates[4]){
  // Follow the line and go forward (main line-following function)
  recordLineValue(lineStates);
  int leftWeight = sumWeight(leftWeightArray);
  int rightWeight = sumWeight(rightWeightArray);
  // Execute recordLineValue and sumWeight functions to compute the weighting for each turn

  // Maneuver the robot based on the states of the line sensors
  if (lineStates[1] == 0 && lineStates[2] == 0){
    forward(speed);
  }
  else if (lineStates[1] == 0 && lineStates[2] == 1){
    follow(speed, speed - rightWeight);
  }
  else if (lineStates[1] == 1 && lineStates[2] == 0){
    follow(speed - leftWeight, speed);
  }
  // Slow down the inner wheel based on the turning weighting, the slowing the wheel the sharper the turn
  // The sum of past line sensor readings represents how much the AGV has to turn to avoid the line
  // The greater the past readings, the sharper the turn
  else{
    Serial.println("Error: both inner line sensors are on the white line");
    forward(speed);
  }
  readLine();
}

void drive_route(int* journey, int number_of_junctions) {
   // Travel from a point to another, based on the route provided
    int journey_count = 0;  
    while (journey_count < number_of_junctions) {

        // Move forward consistently until a junction is reached
        readLine();
        while(lineStates[0] == 0 && lineStates[3] == 0){
            adjust(lineStates);
            readLine();
        }

        stop();
        delay(250);
        // Briefly stop

        if (journey[journey_count] == 0) {
            Serial.println("Going straight");
            forward(speed);
            delay(600);
        }
        else if (journey[journey_count] == 1) { 
            Serial.println("Turning right");
            turn(1);
        }
        else if (journey[journey_count] == 2) {
          Serial.println("Turning left");
            turn(2);
        }
        // If the journey is 0: the AGV moves forward at the junction (goes straight)
        // If the journey is 1: the AGV turns right
        // If the journey is 2: the AGV turns left

        stop();
        delay(250);
        journey_count++;
        // Increment the journey_count within a route
    }
}
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Create the DC motors on ports M1 (left) and M2 (right)
Adafruit_DCMotor *left = AFMS.getMotor(1);
Adafruit_DCMotor *right = AFMS.getMotor(2);

// Pins for the junction sensor inputs
const int left_junction_sensor = /*insert sensor pin*/;
const int right_junction_sensor = /*insert sensor pin*/;

int routes[] = {SA, AG, AR, AB, GB, RB, BG, BR, BC, GC, RC, CG, CR, CD, GD, RD, DG, DR, DS, GS, RS};
int route_lengths[] = {}

// magic code
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - Sensor and Movement Example");
  
  if (!AFMS.begin()) {
    Serial.println("Failed to initialize Adafruit MotorShield!");
    while (1);
  }
  
  AFMS.setPWM(0);  // Disable the motors during setup
  

  // set sensor pins to input
  pinMode(left_junction_sensor, INPUT);
  pinMode(right_junction_sensor, INPUT);
  
}


void drive_route(int* journey, int number_of_junctions) {
   
    int journey_count = 0;  
    while (journey_count < number_of_junctions) {

        // Move forward consistently (need to include line following here)
        forward(255);

        // Check for sensor input
        int left_sensorValue = digitalRead(left_junction_sensor);
        int right_sensorValue = digitalRead(right_junction_sensor);

        // if junction sensed, should probably be done on an interrupt basis
        if (/* insert syntax for sensing a junction */) {
            // Sensor is triggered, stop the motors and perform the required turn
            stopMotors();
            
            if (journey[journey_count] == 0) {
                // robot will continue to move forwards
            }
            else if (journey[journey_count] == 1) { 
                turn(RIGHT);
            }
            else if (journey[journey_count] == 2) {
                turn(LEFT);
            }

            delay(1000);

            journey_count++;  // Fixed the syntax for incrementing journey_count
        }
    }
}


void forward(int speed) {
  // Rotate both motors forward at the given speed, will need to be determined
  left->setSpeed(speed);
  right->setSpeed(speed);
  left->run(FORWARD);
  right->run(FORWARD);
}

void stopMotors() {
  // Release both motors to stop them
  left->run(RELEASE);
  right->run(RELEASE);
}


void turn(int direction) {
  // Function to perform a turn in the specified direction
  // Adjust the motor speeds and run direction based on turning requirements
  left->setSpeed(200);
  right->setSpeed(200);

  if (direction == LEFT) {
    left->run(BACKWARD);
    right->run(FORWARD);
  } else if (direction == RIGHT) {
    left->run(FORWARD);
    right->run(BACKWARD);
  }

  delay(500);  // Adjust the turn duration based on turn requirements again

  // Stop the motors after the turn
  stopMotors();
}




void loop() {

    int route_counter = 0;

    // drive to the first checkpoint
    drive_route(routes[0], route_lengths[0]);

    // check for block
    bool block_present = block_detect();

    // run delivery of block if present
    if (block_present) {
        // detect the block's color
        bool colour_present = colour_detect();

        // pick up the block and return to the track
        pick_up_block();

        // adjust route depending on the platform required
        if (colour_present == black) {
            route_counter += 1;
        } else if (colour_present == red) {
            route_counter += 2;
        }

        // deliver block to the platform and proceed to the next location
        drive_route(routes[route_counter], route_lengths[route_counter]);
        drop_off_block();
        route_counter += 3;
        drive_route(routes[route_counter], route_lengths[route_counter]);
    }

    // if no block detected, proceed to the next station
    else if (!block_present) {
        route_counter += 3;
        drive_route(routes[route_counter], route_lengths[route_counter]);
        route_counter += 2;
    }
}

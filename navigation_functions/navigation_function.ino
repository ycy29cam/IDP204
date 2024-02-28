#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "routes.h"
#include <Adafruit_VL53L0X.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Create the DC motors on ports M1 (left) and M2 (right)
Adafruit_DCMotor *left = AFMS.getMotor(1);
Adafruit_DCMotor *right = AFMS.getMotor(2);

/*
// Pins for the junction sensor inputs
const int left_junction_sensor = insert sensor pin;
const int right_junction_sensor = insert sensor pin;
*/

int routes[17] = {SA, AG, AR, AB, GB, RB, BG, BR, BC, GC, RC, CG, CR, CD, GD, RD, DG, DR, DS, GS, RS};
int route_lengths[17] = {3,1,2,4,6,4,4,5,4,4,3,4,3,3,2,4,3};

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

void approach_block(){
    
    // initialise a block range, this can be moved into a header file later
    int BLOCK_RANGE = 300;

    // lower grabber arm
    lower_grabber();

    // initialize block distance
    Adafruit_VL53L0X block_distance = Adafruit_VL53L0X();

    // begin ranging
    block_distance.begin()
    block_distance.startRangeContinuous();
    
    // advance towards block until in grabbing range
    while (block_distance.readRange() < BLOCK_RANGE){
        readLine();
        adjust(linestates);
        delay(10);
    }
    
    // use picking up block routine (this should include raising the grabber, but not lowering it)
    grab_block();

}



void exit(int route_counter, bool colour_present){
    int station; // it may be useful for this to become a global variable later
    volatile bool junction_detected = false;

    // decide if we're at station ABCD -> 0123
    if (route_counter < 6){
        station = 0;
    }
    else if (route_counter < 11){
        station = 1;
    }
    else if (route_counter < 16){
        station = 2;
    }
    else if (route_counter < 21){
        station = 3;
    }

    // reverse until original junction detected
    while (!junction_detected){
        if (linestates[0]||linestates[3]){
            junction_detected = true;
        }
        else{
            backward(/*at some slow speed*/);
        }
    }

    // drive forwards a specified distance until axle is on original line
    forward(/*time*/);

    // turn 90 degrees clockwise for (A+B).R + G.(!A)
    if (((station == 0)||(station == 1))&&((colour_present))||((station != 0)&&(!colour_present))){
        //turn 90 degrees clockwise
    }
    else{
        //turn 90 degrees anticlockwise
    }



}


bool block_detect(){

    while(){
        readLine();
        adjust(linestates);
    }



}


void drive_route(int* journey, int number_of_junctions) {
   
    int journey_count = 0;  
    while (journey_count < number_of_junctions) {

        // Move forward consistently
        readLine();
        adjust(linestates);

        // Check for sensor input
        /*
        int left_sensorValue = digitalRead(left_junction_sensor);
        int right_sensorValue = digitalRead(right_junction_sensor);
        */

        // if junction sensed, should probably be done on an interrupt basis
        if ((linestates[0])||(linestates[3])) {
            
            // Sensor is triggered, stop the motors and perform the required turn
            stopMotors();
            
            if (journey[journey_count] == 0) {
                // robot will continue to move forwards
            }
            else if (journey[journey_count] == 1) { 
                turnRight(/*duration*/);
            }
            else if (journey[journey_count] == 2) {
                turnLeft();
            }
            else if (journey[journey_count] == 3) {
                ArcTurnRight();
            }
            else if (journey[journey_count] == 4) {
                ArcTurnLeft(/*duration*/);
            }
            delay(1000);

            journey_count++;  // increment along the specific journey
        }
    }
}


// main loop to be run for navigation
void loop() {

    int route_counter = 0;

    // drive to the first checkpoint
    drive_route(routes[0], route_lengths[0]);

    // check for block
    bool block_present = block_detect();

    // run delivery of block if present (may have to remove this functionality if there is always going to be a block present)
    if (block_present) {

        // approach the block and pick it up
        approach_block(); 

        // detect the block's color
        bool colour_present = colour_detect();

        // exit station: reverse out and make a 90 degree turn with direction decided by station and colour
        exit(route_counter, colour_present);

        // adjust route depending on the platform required
        // black block returns false, red block returns true
        if (!colour_present) {
            route_counter += 1;
        } else if (colour_present) {
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

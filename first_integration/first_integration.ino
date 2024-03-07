#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"
DFRobot_VL53L0X sensor;
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include "movements.h"
#include "routes.h"
#include "block_handle.h"

int button = 13; // Button
int routeNumber = 17; // Total number of routes

void setup() {
  Serial.begin(9600);

  // Set up the time of flight distance sensor
  Wire.begin();
  sensor.begin(0x50);
  sensor.setMode(sensor.eContinuous,sensor.eHigh);
  sensor.start();

  pinMode(LOut, INPUT);
  pinMode(LIn, INPUT);
  pinMode(RIn, INPUT);
  pinMode(ROut, INPUT);
  pinMode(colour_sensor,INPUT);
  pinMode(button, INPUT);
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  // Declare all inputs and outputs

  myservo1.attach(9);      // Servo input at pin 9
  myservo2.attach(10);     // Servo input at pin 10

  myservo1.write(110);     // Define initial position
  myservo2.write(65);      // Define initial position

  // Magical code (but why?)
  // https://forum.arduino.cc/t/my-void-setup-is-repeating/669468/7 (potential reason?)
  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
}

void loop() {
  /*
  while(digitalRead(button) == 0){
    // Wait for the start button to the pressed
    delay(100);
  }
  */

  Serial.print("Begin");

  while(routeCounter < routeNumber){
    // While the current route index is in range of the 2D array
    drive_route(routes[routeCounter], route_lengths[routeCounter]);
    // Drive the robot according to the route, to a pick up point, from either the start or the drop off points

    if (routeCounter > 14){
      stop();
      Serial.println("Exercise ended");
    }

    readLine();
    while (lineStates[0] == 0 && lineStates[3] == 0){
      adjust(lineStates);
      readLine();
    }

    stop();
    delay(250);

// approach the block and pick it up
    approach_block(final_turns[routeCounter]); // Direction to be specified

    Serial.print("Before adjustment:");
    Serial.println(routeCounter);
    if (routeCounter >= 3){checkRoute(previous_colour); }
    Serial.print("After adjustment:");
    Serial.println(routeCounter);
    
    // detect the block's color
    // black block returns false, red block returns true
    bool colour_present = colour_detect();
    previous_colour = colour_present;

    // exit station: reverse out and make a 90 degree turn with direction decided by station and colour
    // adjust route depending on the platform required
    leave(colour_present); // Increment routeCounter by 1 if black, 2 if red

    drive_route(routes[routeCounter], route_lengths[routeCounter]);
    dropOffBlock(colour_present); // Increment routeCounter by 2 (drop off point to the next pick up point)

    routeCounter += 2;
  }
}
// Not done Not done Not done


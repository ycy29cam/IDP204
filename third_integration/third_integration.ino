// Please note that team L204 competed in the IDP competition on 13/3/2024 with the AGV running third_integration
// Therefore, only the third_integration is submitted as a documented reference of the work completed by the software division

#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"
DFRobot_VL53L0X sensor;
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include "movements.h"
#include "routes.h"
#include "block_handle.h"
// Import all necessary libraries (Arduino, Wire, DFRobot_VL53L0X, Adafruit_MotorShield, Servo) and header files (movements, routes, block_handle)

int button = 13; 
// Declare the pin number for the button
int routeNumber = 17; 
// Declare the total number of routes

void setup() {
  Serial.begin(9600);
  // Set up the serial connection, for debugging

  Wire.begin();
  sensor.begin(0x50);
  sensor.setMode(sensor.eContinuous,sensor.eHigh);
  sensor.start();
  // Set up the time-of-flight distance sensor

  pinMode(LOut, INPUT);
  pinMode(LIn, INPUT);
  pinMode(RIn, INPUT);
  pinMode(ROut, INPUT);
  pinMode(colour_sensor,INPUT);
  pinMode(button, INPUT);
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  // Declare all inputs and outputs (some pin numbers are declared within the header files)

  myservo1.attach(9);      // Servo output at pin 9
  myservo2.attach(10);     // Servo output at pin 10

  myservo1.write(120);     // Define initial position of the grabber
  myservo2.write(85);      // Define initial position of the grabber arm

  // Magical code
  // https://forum.arduino.cc/t/my-void-setup-is-repeating/669468/7 
  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
}

void loop() {
  
  while(digitalRead(button) == 0){
    // Wait for the start button to the pressed
    delay(100);
  }

  Serial.print("Begin");
  while(1){
    while(routeCounter < routeNumber){
      // While the current route index is in range of the 2D array
      drive_route(routes[routeCounter], route_lengths[routeCounter]);
      // Drive the AGV according to the route, to a pick-up point, from either the start or the drop off points

      if (routeCounter > 14){
        // Terminate the while loop when the AGV approaches the end of the competition cycle
        while (lineStates[0] == 0 && lineStates[3] == 0){
            readLine();
            adjust(lineStates);
        }
        // Follow the line and move towards the starting box, until the border line is detected
        forward(speed);
        delay(1000);
        stop();
        Serial.println("Exercise ended");
        delay(10000000);
        // Move the AGV into the starting box and stop, with an essentially inifite delay
      }

      readLine();
      // Take initial readings from the line sensors, which are stored in the variable lineStates[4]
      while (lineStates[0] == 0 && lineStates[3] == 0){
        adjust(lineStates);
        readLine();
        // Follow the line and move towards the next junction (junction before a pick-up point), until either of the junction sensors is triggered
      }

      stop();
      delay(250);
      // Briefly stop at the junctions

      approach_block(final_turns[routeCounter]);
      // Execute the approach_block function which approaches the block and pick it up

      Serial.print("Before adjustment:");
      Serial.println(routeCounter);
      if (routeCounter >= 3){checkRoute(previous_colour); }
      Serial.print("After adjustment:");
      Serial.println(routeCounter);
      // Make correction to the routeCounter to synchronise the index in the route array, representing each pick-up point
      
      bool colour_present = colour_detect();
      previous_colour = colour_present;
      // Detect the block's colour
      // Store the current colour, to be used to adjust the routeCounter in the following cycle

      leave(colour_present);
      // Execute the leave function which lights up the corresponding LED according to the block colour and reverse the AGV

      drive_route(routes[routeCounter], route_lengths[routeCounter]);
      // Drive the AGV according to the route, to the correct drop-off point, from one of the pick-up points
      dropOffBlock(colour_present);
      // Execute the dropOffBlock function which approaches the drop-off platform, drops the block and performs a 180 degrees turn

      routeCounter += 2;
      // Increment routeCounter by 2 to proceed to the correct route
    }
  }
}
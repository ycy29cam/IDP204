#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "routes.h"
#include <Adafruit_VL53L0X.h>

void pick_up_block(int route_counter, bool colour_present){
    
    volatile bool junction_detected = 0;
    int station; // it may be useful for this to become a global variable later

    // decide if we're at station ABCD -> 0123
    if (route_counter < 6){
        station = 0
    }
    else if (route_counter < 11){
        station = 1
    }
    else if (route_counter < 16){
        station = 2
    }
    else if (route_counter < 21){
        station = 3
    }

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
    
    // use picking up block routine
    /*grab_block();*/

    // reverse until original junction detected
    while (!junction_detected){
        if (linestates[0]||linestates[3]){
            junction_detected = 1;
        }
        else{
            backward(/*at some slow speed*/);
        }
    }

    // drive forwards a specified distance until axle is on original line
    forward(/*time*/);

    // turn 90 degrees clockwise for B + (RED).(!D) + D.(BLACK)
    if ((station == 1)||((station != 3)&&(colour_present))||((station == 3)&&(!colour_present))){
        //turn 90 degrees clockwise
    }
    else{
        //turn 90 degrees anticlockwise
    }

}


void loop(){
    pick_up_block(3, TRUE);
}
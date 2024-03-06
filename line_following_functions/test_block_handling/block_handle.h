#include <Servo.h>          // Need to include this at the top of the file
Servo myservo;              // create servo object to control a servo

int InfraredSensorPin = 8;

/**void approach_block(int direction){

    forward(600);

    if (direction == 1){
        turnRight();
        delay(500);
        readLine();
        while(lineStates[1] == 0){
            turnRight();
            readLine();
        }
    }
    else if (direction == 2){
        turnLeft();
        delay(500);
        readLine();
        while(lineStates[2] == 0){
            turnLeft();
            readLine();
        }
    }
    else {
        Serial.println("Error, no turn indicated")
        forward(speed);
        delay(500);
    }
    
    // initialise a block range, this can be moved into a header file later
    int BLOCK_RANGE = 500;

    // lower grabber arm
    // CURRENTLY NOT PROGRAMMED
    lower_grabber(); // Not needed if the grabber is down by default, after a drop off

    // initialize block distance
    Adafruit_VL53L0X block_distance = Adafruit_VL53L0X();

    // begin ranging
    block_distance.begin()
    block_distance.startRangeContinuous();
    
    // advance towards block until in grabbing range
    while (block_distance.readRange() > BLOCK_RANGE){
        readLine();
        adjust(linestates);
        delay(10); //Probably don't need this
    }

    stop();
    
    // use picking up block routine (this should include raising the grabber, but not lowering it)
    // CURRENTLY NOT PROGRAMMED
    grab_block();
}

// colour detection function
bool colour_detect(){

    // red returns true, black returns false
    if (digitalRead(InfraredSensorPin)){
        return true;
    }
    else{
        return false;
    }
}

void leave(bool colour_present){
    int station; // it may be useful for this to become a global variable later

    // decide if we're at station ABCD -> 0123
    if (routeCounter < 6){
        station = 0;
    }
    else if (routeCounter < 11){
        station = 1;
    }
    else if (routeCounter < 16){
        station = 2;
    }
    else if (routeCounter < 21){
        station = 3;
    }

   readLine();
   while(lineStates[0] == 0 && lineStates[3] == 0){
    backward(speed);
    readLine();
   }

    // turn 90 degrees clockwise for (A+B).R + G.(!A)
    if ((((station == 0)||(station == 1))&&((colour_present)))||((station != 0)&&(!colour_present))){
        //turn 90 degrees clockwise
        arcTurnRight();
    }
    else{
        //turn 90 degrees anticlockwise
        arcTurnLeft();
    }

    if (!colour_present) {
        routeCounter += 1;
    } 
    else if (colour_present) {
        routeCounter += 2;
    }
}
**/

void rotate_arms_to(int n){                       //Function to contract the grabber arms
  myservo.attach(9);                          //90 is the down positions and 150 is the up position for the grabber arms
  myservo.write(n);
  delay(15);                         
}

void lift_arms(){                           //Function to lift arms to raised position
  rotate_arms_to(150);
  delay(1000);
}

void lower_arms(){                          //Function to lower arms to bottom position
  rotate_arms_to(90);
  delay(1000);
}

void close_arms(){                        //Function to close grabber arms from open position
  rotate_arms_to(60);
  delay(1000);
}

void open_arms(){                         //Function to open grabber arms from closed position
  rotate_arms_to(110);
  delay(1000);
}

//void set_block_position(){


/**void tellColour(int colour_present){      //Function to detect colour of block
    if(!colour_present){
        digitalWrite(green, HIGH);
    }
    else if (colour_present){
        digitalWrite(red, HIGH);
    }
}

void turnOffLED(){                             //
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
}**/
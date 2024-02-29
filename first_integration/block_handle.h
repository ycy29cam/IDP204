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
}

void approach_block(int direction){

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
        delay(10); //Probably don't need this
    }
    
    // use picking up block routine (this should include raising the grabber, but not lowering it)
    // CURRENTLY NOT PROGRAMMED
    grab_block();
}

// colour detection function
bool colour_detect(){
    const int InfraredSensorPin = 8;
    pinMode(InfraredSensorPin,INPUT);

    // red returns true, black returns false
    if (digitalRead(InfraredSensorPin)){
        return true;
    }
    else{
        return false;
    }
}
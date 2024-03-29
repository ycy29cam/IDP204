int InfraredSensorPin = 8;
long startLine;


void tellColour(int colour_present){
    if(!colour_present){
        digitalWrite(green, HIGH);
    }
    else if (colour_present){
        digitalWrite(red, HIGH);
    }
}

void turnOffLED(){
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
}

void approach_block(int direction){
  // Has been simplified for testing

    Serial.println("Approaching pick up");
    forward(speed);
    delay(500);
    Serial.println("Moved forward");

    if (direction == 1){
      Serial.println("Turn right into pick up");
        turnRight();
        delay(800);
        readLine();
        while(lineStates[1] == 0){
            turnRight();
            readLine();
        }
    }
    else if (direction == 2){
      Serial.println("Turn left into pick up");
        turnLeft();
        delay(800);
        readLine();
        while(lineStates[2] == 0){
            turnLeft();
            readLine();
        }
    }
    else {
        Serial.println("Not turning");
        forward(speed);
        delay(250);
    }

    startLine = millis();

    while((millis() - startLine) < 1000){
    adjust(lineStates);
    readLine();
    }

    stop();
    
    /*
    // initialise a block range, this can be moved into a header file later
    int BLOCK_RANGE = 500; // Change later

    // lower grabber arm
    // CURRENTLY NOT PROGRAMMED
    lower_grabber(); // Not needed if the grabber is down by default, after a drop off

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
    */
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

void leave(bool colour_present=true){
  // Red colour detected by default, for testing
    int station; // it may be useful for this to become a global variable later
    Serial.print("Currently in station");
    Serial.println(station);

    // decide if we're at station ABCD -> 0123
    Serial.print("routeCounter as read in leave function:");
    Serial.println(routeCounter);
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

    delay(5000);
    turnOffLED();

   readLine();
   while(lineStates[0] == 0 && lineStates[3] == 0){
    backward(speed);
    readLine();
   }

   stop();
   delay(250);

    // turn 90 degrees clockwise for (A+B).R + G.(!A)
    if ((((station == 0)||(station == 1))&&((colour_present)))||((station != 0)&&(!colour_present))){
        //turn 90 degrees clockwise
        Serial.println("After picking up a block, I am turning right");
        turn(1);
    }
    else{
        //turn 90 degrees anticlockwise
        Serial.println("After picking up a block, I am turning left");
        turn(2);
    }

    if (!colour_present) {
        routeCounter += 1;
    } 
    else if (colour_present) {
        routeCounter += 2;
    }
}

void checkRoute(bool colour_present){
    if (!colour_present){
        routeCounter += 1;
    }
}
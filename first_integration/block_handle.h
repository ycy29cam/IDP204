Servo myservo1;              // create servo object to control a servo
Servo myservo2;
int colour_sensor = 11;
bool colour_present;

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

// Definition of grabber functions
void rotate_arms_to_1(int n){                    //Function to contract the grabber arms
      myservo1.write(n);                             //60 is closed and 110 is open position for the grabber
      delay(15);                         
}

void rotate_arms_to_2(int n){                    //Function to contract the grabber arms   
      myservo2.write(n);                         //18 is the down positions and 65 is the up position for the grabber arms                       
      delay(15);                         
}

void lift_arms(){                           //Function to lift arms to raised position
  int pos = 18;  
  for (pos = 18; pos <= 65; pos += 1) {    
    rotate_arms_to_2(pos);
    delay(50); 
 }
  delay(1000);
}

void lower_arms(){                          //Function to lower arms to bottom position
  int pos = 65;  
  for (pos = 65; pos >= 18; pos -= 1) { 
    rotate_arms_to_2(pos);
    delay(50); 
 }
  delay(1000);
}

void close_arms(){                        //Function to close grabber arms from open position
      rotate_arms_to_1(60);                       
      delay(1000);
}

void open_arms(){                         //Function to open grabber arms from closed position
      rotate_arms_to_1(110);
      delay(1000);
}

// End of grabber function definition


void approach_block(int direction){

    forward(speed);
    delay(600);

    if (direction == 1){
      Serial.println("Turning right into pick up");
        turnRight();
        delay(1200);
        readLine();
        while(lineStates[1] == 0){
            turnRight();
            readLine();
        }
    }
    else if (direction == 2){
      Serial.println("Turning left into pick up");
        turnLeft();
        delay(1200);
        readLine();
        while(lineStates[2] == 0){
            turnLeft();
            readLine();
        }
    }
    else {
        forward(speed);
        delay(250);
    }

    stop();
    Serial.println("Pick up");
    
    // initialise a block range, this can be moved into a header file later
    int BLOCK_RANGE = 100;

    // lower grabber arm
    // CURRENTLY NOT PROGRAMMED
    lower_arms(); // Not needed if the grabber is down by default, after a drop off
    open_arms();

    // initialize block distance
    
    // advance towards block until in grabbing range

    Serial.print("Distance is: ");
    Serial.println(sensor.getDistance());
    readLine();
    while (sensor.getDistance() > BLOCK_RANGE){
        Serial.print("Distance is: ");
        Serial.println(sensor.getDistance());
        adjust(lineStates);
        readLine();
    }

    stop();
    
    // use picking up block routine (this should include raising the grabber, but not lowering it)
    // CURRENTLY NOT PROGRAMMED
    close_arms();
    lift_arms();
}

// colour detection function
bool colour_detect(){
    // red returns true, black returns false
    if (digitalRead(colour_sensor)){
        Serial.println("Colour is red");
        return true;
    }
    else{
        Serial.println("Colour is black");
        return false;
    }
}

void leave(bool colour_present){
    int station; // it may be useful for this to become a global variable later

    // decide if we're at station ABCD -> 0123
    /*
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
    */
    if (routeCounter < 3){
        station = 0;
    }
    else if (routeCounter < 7){
        station = 1;
    }
    else if (routeCounter < 11){
        station = 2;
    }
    else if (routeCounter < 18){
        station = 3;
    }
    // Stations redefined, since the total number of routes is 17, instead of 21

    tellColour(colour_present); // Light up the corresponding LED
    delay(5000);
    turnOffLED();

   readLine();
   while(lineStates[0] == 0 && lineStates[3] == 0){
    backward(speed);
    readLine();
   }

    /* // Sorry Olly, we can't get the rounded corners! :(
    // turn 90 degrees clockwise for (A+B).R + G.(!A)
    if ((((station == 0)||(station == 1))&&((colour_present)))||((station != 0)&&(!colour_present))){
        //turn 90 degrees clockwise
        Serial.println("After pickup, turning right");
        turn(1);
    }
    else{
        //turn 90 degrees anticlockwise
        Serial.println("After pickup, turning left");
        turn(2);
    }

    if (!colour_present) {
        routeCounter += 1;
    } 
    else if (colour_present) {
        routeCounter += 2;
    }
    */
    if (station == 0 && colour_present){
      turn(1);
    }
    else if (station == 0 && !colour_present){
      turn(2);
    }
    else if (station == 1 && colour_present){
      turn(2);
    }
    else if (station == 1 && !colour_present){
      turn(2);
    }
    else if (station == 2 && colour_present){
      turn(1);
    }
    else if (station == 2 && !colour_present){
      turn(1);
    }
    else if (station == 3 && colour_present){
      turn(2);
    }
    else if (station == 3 && !colour_present){
      turn(1);
    }
    // Hard-coding the boolean algebra since the rounded corners are ignored, sorry Olly

    if (!colour_present) {
        routeCounter += 1;
    } 
    else if (colour_present) {
        routeCounter += 2;
    }
}

void dropOffBlock(bool colour_present){

    readLine();
    while(lineStates[0] == 0 && lineStates[3] == 0){
        adjust(lineStates);
        readLine();
    }

    stop();

    lower_arms();
    open_arms();
    delay(500);
    lift_arms();

    backward(speed);
    delay(600);

    if (colour_present){
      Serial.println("After dropoff, turning right");
        turnRight();
        delay(300);
        readLine();
        while (lineStates[1] == 0){
            turnRight();
            readLine();
        }
    }
    else {
      Serial.println("After droppoff, turning left");
        turnLeft();
        delay(300);
        readLine();
        while (lineStates[2] == 0){
            turnLeft();
            readLine();
        }
    }

    stop();
    delay(250);

}
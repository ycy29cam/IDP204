// A header file storing all variables and functions related to picking up and dropping off blocks

Servo myservo1;
Servo myservo2;
// Create servo objects
int colour_sensor = 11;
// Declare the pin number for the colour sensor
bool colour_present;
// Declare a boolean variable for storing the colour of the block (red = True, black = False)

void tellColour(int colour_present){
  // Lights up the corresponding LED based on the block colour detected
  // Red block = red LED; black block = green LED
    if(!colour_present){
        digitalWrite(green, HIGH);
    }
    else if (colour_present){
        digitalWrite(red, HIGH);
    }
}

void turnOffLED(){
  // Switches off the LEDs 
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
}

void rotate_arms_to_1(int n){                    
  // Function to define the position of the grabber
      myservo1.write(n);                             
      //60 is closed and 110 is open (for the grabber)
      delay(15);                         
}

void rotate_arms_to_2(int n){                    
  // Function to define the position of the grabber arms
      myservo2.write(n);                         
      //30 is the down position and 85 is the up position (for the grabber arms)                      
      delay(15);                         
}

void lift_arms(){                           
  // Raises the grabber arms
  int pos = 32;  
  for (pos = 32; pos <= 85; pos += 1) {    
    rotate_arms_to_2(pos);
    delay(20); 
 }
  delay(1000);
}

void lift_arms_less(){                           
  // A more gentle version of the lift_arms function (for the ease of balancing the AGV, since the grabber arms contribute a moment)
  int pos = 45;  
  for (pos = 45; pos <= 85; pos += 1) {    
    rotate_arms_to_2(pos);
    delay(20); 
 }
  delay(1000);
}

void lower_arms(){                          
  // Lowers the grabber arms
  int pos = 85;  
  for (pos = 85; pos >= 32; pos -= 1) { 
    rotate_arms_to_2(pos);
    delay(20); 
 }
  delay(1000);
}

void lower_arms_less(){                        
  // A more gentle version of the lower_arms function (for the ease of balancing the AGV, since the grabber arms contribute a moment)
  int pos = 85;  
  for (pos = 85; pos >= 45; pos -= 1) { 
    rotate_arms_to_2(pos);
    delay(20); 
 }
  delay(1000);
}


void close_arms(){                        
  // Closes the grabber, to grab a block
      rotate_arms_to_1(60);                       
      delay(1000);
}

void open_arms(){                         
  // Opens the grabber, to release a block
      rotate_arms_to_1(120);
      delay(1000);
}

void approach_block(int direction){
  // Turns the AGV into the pick-up point, approaches the block and picks up the block

    forward(speed);
    delay(300);
    // Move forward for 300ms to approach the junction

    if (direction == 1){
      // If the pick-up point is on the right, make a 90 degrees right turn 
      Serial.println("Turning right into pick up");
        turnRight();
        delay(500);
        readLine();
        while(lineStates[1] == 0){
            turnRight();
            readLine();
        }
    }
    else if (direction == 2){
      // If the pick-up point is on the left, make a 90 degrees left turn
      Serial.println("Turning left into pick up");
        turnLeft();
        delay(500);
        readLine();
        while(lineStates[2] == 0){
            turnLeft();
            readLine();
        }
    }
    else {
      // If no direction is specified, move forward to avoid the junction
        forward(speed);
        delay(250);
    }

    stop();
    Serial.println("Pick up");
    // Briefly stop the AGV
    
    int BLOCK_RANGE = 90;
    // Initiate the expected distance of the block

    lower_arms();
    open_arms();
    // Execute the lower_arms and open_arms functions to lower the grabber arms and open the grabber

    Serial.print("Distance is: ");
    Serial.println(sensor.getDistance());
    readLine();
    while (sensor.getDistance() > BLOCK_RANGE){
        Serial.print("Distance is: ");
        Serial.println(sensor.getDistance());
        adjust(lineStates);
        readLine();
        // Follow the line until the AGV is closer to the block than the specified BLOCK_RANGE
    }

    stop();
    // Stop the AGV during the pick-up process

    close_arms();
    lift_arms();
    // Execute the close_arms and lift_arms functions to close the grabber and raise the grabber arms
}

// colour detection function
bool colour_detect(){
    // Takes reading for the colour sensor pin and return True if the block is red; False if the block is black
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
    int station;
    // Declare a variable for storing the current station number

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
    // Determine the current station (ABCD -> 0123)

    tellColour(colour_present);
    delay(5000);
    turnOffLED();
    // Execute tellColour and turnOffLED functions to light up the indicating LED for 5s

   readLine();
   while(lineStates[0] == 0 && lineStates[3] == 0){
    backward(speed);
    readLine();
   }
   // Reverse the AGV until the junction is sensed

    // Turn 90 degrees clockwise for (A+B).R + G.(!A)
    if ((((station == 0)||(station == 1))&&((colour_present)))||((station != 0)&&(!colour_present))){
        // Turn 90 degrees clockwise
        Serial.println("After pickup, turning right");
        turn(1);
    }
    else{
        // Turn 90 degrees anticlockwise
        Serial.println("After pickup, turning left");
        turn(2);
    }

    if (!colour_present) {
        routeCounter += 1;
    } 
    else if (colour_present) {
        routeCounter += 2;
    }
    // Increment routeCounter according to the block colour, to select the correct route to the drop-off point
}

void dropOffBlock(bool colour_present){

    lower_arms_less();
    // Execute lower_arms_less to lower the grabber arms before the AGV approaches the drop-off platform

    readLine();
    long dropOffTime = millis();
    while(millis() - dropOffTime < 1500){
        adjust(lineStates);
        readLine();
    }
    // Move the AGV forward towards the platform for 1.5s

    stop();
    // Stop the AGV to drop the block off

    open_arms();
    lift_arms_less();
    // Execute the open_arms and lift_arms_less functions to open the grabber (drop the block) and raise the grabber arms

    backward(speed);
    delay(400);
    // Move the AGV slightly backward (to avoid hitting the drop-off platform while making the 180 degrees turn)

    if (colour_present){
      // Turning 180 degrees to the right at the red drop-off point (AGV runs into the wall if it turns left)
      Serial.println("After dropoff, turning right");
        turnRight();
        delay(800);
        readLine();
        while (lineStates[1] == 0){
            turnRight();
            readLine();
        }
    }
    else {
      // Turning 180 degrees to the left at the green drop-off point (AGV runs into the wall if it turns right)
      Serial.println("After droppoff, turning left");
        turnLeft();
        delay(800);
        readLine();
        while (lineStates[2] == 0){
            turnLeft();
            readLine();
        }
    }

    stop();
    delay(250);
    // Briefly stop before travelling to the next pick-up point
}
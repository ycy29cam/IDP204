int LOut = 1, LIn = 2, RIn = 3, ROut = 4;
// Declare pins used by the 4 line sensors
int lineStates[4];
// Declare a list to store the 4 states of 4 separate line sensors

void setup(){
  Serial.begin(9600);

  pinMode(LOut, INPUT);
  pinMode(LIn, INPUT);
  pinMode(RIn, INPUT);
  pinMode(ROut, INPUT);
}

void readLine(){
  lineStates[0] = digitalRead(LOut);
  lineStates[1] = digitalRead(LIn);
  lineStates[2] = digitalRead(RIn);
  lineStates[3] = digitalRead(ROut);
}


void loop(){
  readLine();
  Serial.print(lineStates[0]);
  Serial.print(lineStates[1]);
  Serial.print(lineStates[2]);
  Serial.print(lineStates[3]);
  Serial.println("-------------------------");
  delay(500);
}


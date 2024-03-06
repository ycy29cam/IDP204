int colour_sensor = 11;

void setup() {
  Serial.begin(9600);

  pinMode(colour_sensor, INPUT);
}

void loop() {
  if(digitalRead(colour_sensor)){
    Serial.println("Red");
    Serial.println("-------------------");
  }
  else{
    Serial.println("Black");
    Serial.println("-------------------");
  }
  delay(500);
}

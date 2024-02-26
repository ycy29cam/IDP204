int button = 1;

void setup(){
  Serial.begin(9600);
  pinMode(button, INPUT);
}

void loop(){
  int state = digitalRead(button);
  if (state == HIGH){
    Serial.println("ON");
  }
  else{
    Serial.println("OFF");
  }
  delay(500);
}

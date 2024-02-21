int blue = 5;
int red = 6;
int green = 7;

void setup() {
  pinMode(blue, OUTPUT);
  pinMOde(red, OUTPUT);
  pinMode(green, OUTPUT);
}

void loop() {
  digitalWrite(blue, HIGH);
  delay(1000);
  digitalWrite(blue, LOW);
  digitalWrite(red, HIGH);
  delay(1000);
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
  delay(1000);
  digitalWrite(green, LOW);
}

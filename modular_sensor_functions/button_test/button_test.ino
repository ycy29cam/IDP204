int button = 0;

void setup() {
  Serial.begin(9600);

  pinMode(button, INPUT);
}

void loop() {
  bool reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {
        // Assuming the button is connected to GND and pin 1
        toggleState = !toggleState;
        Serial.println(toggleState ? "ON" : "OFF");
      }
    }
  }

  lastButtonState = reading;
}

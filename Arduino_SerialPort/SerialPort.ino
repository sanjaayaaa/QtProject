const int ledPin = 13; // Pin connected to LED
void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    
    if (inputString == "ON") {
      digitalWrite(ledPin, HIGH); // Turn on the LED
      Serial.println("LED is ON");
    } 
    else if (inputString == "OFF") {
      digitalWrite(ledPin, LOW);
      Serial.println("LED is OFF");
    }
  }
}
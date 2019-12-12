#define doorPin 2

void setup() {
  pinMode(doorPin, INPUT);

}

void loop() {
  Serial.println(digitalRead(doorPin));

}

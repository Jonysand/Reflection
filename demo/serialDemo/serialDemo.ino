byte pixelIndex = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  if(Serial.available()){
    pixelIndex = Serial.parseInt();
    digitalWrite(2, HIGH);
    analogWrite(3, pixelIndex);
  }
}

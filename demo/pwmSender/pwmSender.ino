char mystr[6] = "Hello"; //String data

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.write(mystr,5);
  delay(1000);
}

#define phonePin A6
#define bookPin A7

void setup() {
  

}

void loop() {
  Serial.print("phone: ");
  Serial.println(analogRead(phonePin));

  Serial.print("book: ");
  Serial.println(analogRead(bookPin));

  Serial.println("");
}

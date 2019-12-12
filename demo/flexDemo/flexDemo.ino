const byte howMany = 100;
int readings[howMany];
int readIndex = 0;
int total = 0;
int serialCount = 0;
int sensorValue = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN , OUTPUT);
}

void loop() {
  total -= readings[readIndex];
  readings[readIndex] = analogRead(A7);
  total += readings[readIndex];
  readIndex++;
  if(readIndex>=howMany){
    readIndex = 0;
  }
  sensorValue = total/howMany;
  
  Serial.println(sensorValue);
  
  if (sensorValue<900) {
    digitalWrite(LED_BUILTIN, HIGH);
  }else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}

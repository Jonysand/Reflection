int preState = 560;
int tempState = 560;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
}

void loop() {
  tempState = analogRead(A0);
  Serial.println(tempState);

  if((tempState-preState)>100){
    preState = tempState;
    digitalWrite(2, HIGH);
  }

  if((tempState-preState)<-100){
    preState = tempState;
    digitalWrite(2, LOW);
  }
  
}

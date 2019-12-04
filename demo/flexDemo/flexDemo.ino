const byte howMany = 100;
int readings[howMany];
int readIndex = 0;
int total = 0;
int serialCount = 0;

int state = -1; // -1 for unseated, 1 for seated

int average = 0;
int pre = 0;

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
}

void loop() {
  total -= readings[readIndex];
  
  //read from the sensor
  readings[readIndex] = analogRead(A7);

  //add to the total
  total += readings[readIndex];

  //advance to the next place in array
  readIndex++;

  if(readIndex>=howMany){
    readIndex = 0;
  }
  average = total/howMany;
  
  Serial.println(average - pre);

  if(average>540 && state<0){
    state *= (-1);
  }else if(average<540 && state>0){
    state *= (-1);
  }

  if(state>0){
    digitalWrite(2, HIGH);
  }else{
    digitalWrite(2, LOW);
  }
  
}

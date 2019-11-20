#define redPin 3
#define greenPin 5
#define bluePin 6
int redV = 87;
int greenV = 6;
int blueV = 140;
int redF = 1;
int greenF = 1;
int blueF = 1;

int count = 0;

void setup() {
  pinMode(redPin,OUTPUT);
  pinMode(bluePin,OUTPUT);
  pinMode(greenPin, OUTPUT);
}

void loop() {
  switch(++count%3){
    case 0: analogWrite(redPin, redV);break;
    case 1: analogWrite(greenPin, greenV);break;
    case 2: analogWrite(bluePin, blueV);break;
    default:break;
  }
  redV += 2*redF;
  greenV += 4*greenF;
  blueV += 6*blueF;
  if (redV>255 || redV<0) redF*=(-1);
  if (greenV>255 || greenV<0) greenF*=(-1);
  if (blueV>255 || blueV<0) blueF*=(-1);
  delay(10);
}

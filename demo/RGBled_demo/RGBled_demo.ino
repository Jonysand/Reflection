#include <Adafruit_NeoPixel.h>

#define LED_PIN    9
#define LED_COUNT 84
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
uint32_t background;
#define redPin 12
#define greenPin 11
#define bluePin 10
byte redV = 87;
byte greenV = 6;
byte blueV = 140;
int redF = 1;
int greenF = 1;
int blueF = 1;

int count = 0;

void setup() {
  Serial.begin(9600);
  background = strip.Color(255, 0, 0);
  Serial.println(int(background));
  analogWrite(redPin, 255 - lowByte(background>>16));
  analogWrite(greenPin, 255 - lowByte(background>>8));
  analogWrite(bluePin, 255 - lowByte(background));
}

void loop() {
//  switch(++count%3){
//    case 0: analogWrite(redPin, redV);break;
//    case 1: analogWrite(greenPin, greenV);break;
//    case 2: analogWrite(bluePin, blueV);break;
//    default:break;
//  }
//  redV += 2*redF;
//  greenV += 4*greenF;
//  blueV += 6*blueF;
//  if (redV>255 || redV<0) redV*=(0);
//  if (greenV>255 || greenV<0) greenF*=(-1);
//  if (blueV>255 || blueV<0) blueF*=(-1);
//  delay(10);
}

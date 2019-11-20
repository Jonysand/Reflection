/*
Adafruit Arduino - Lesson 16. Stepper
*/

#include <Stepper.h>

int in1Pin = 9;
int in2Pin = 10;
int in3Pin = 11;
int in4Pin = 12;

Stepper motor(768, in1Pin, in2Pin, in3Pin, in4Pin);  

void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);

  // this line is for Leonardo's, it delays the serial interface
  // until the terminal window is opened
  
  Serial.begin(9600);
  motor.setSpeed(20);
}

void loop()
{
  motor.step(1);
  Serial.println("step");
  delay(1000);
}

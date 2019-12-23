/*
 Controlling a servo position using a potentiometer (variable resistor)
 by Michal Rinott <http://people.interaction-ivrea.it/m.rinott>

 modified on 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Knob
*/

#include <Servo.h>
#define chairPin1 3
#define chairPin2 5
Servo myservo1;
Servo myservo2;

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup() {
  myservo1.attach(chairPin1);
  myservo2.attach(chairPin2);
  myservo1.write(0);
  myservo2.write(0);
}

void loop() {
//  myservo1.write(0);
  myservo2.write(60);
  delay(1000);
//  myservo1.write(120);
  myservo2.write(0);
  delay(1000);
}

#include <Adafruit_NeoPixel.h>
int testFlag = 1;
int state = 3;
int prestate = 3;
int delayTime = 40;

#define ledRpin 12
#define ledGpin 11
#define ledBpin 10

//----------
// LED strip
//----------
#define LED_PIN    6
#define LED_COUNT 84
float brightness = 255.0;
float brightAdd = 1;
uint32_t background;
uint32_t bookColor;
uint32_t phoneColor;
uint32_t treeColor;
int radius = 0;
float thisColor = 0;


//----------------
// ceiling LED
//----------------
#define ledRpin 12
#define ledGpin 11
#define ledBpin 10
int Rvalue = 255;
int Gvalue = 255;
int Bvalue = 255;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  //Serial
  Serial.begin(9600);

  // LED strip
  strip.begin();
  background = strip.gamma32(strip.Color(255, 255, 255));
  bookColor = strip.gamma32(strip.Color(223, 196, 255));
  phoneColor = strip.gamma32(strip.Color(255, 0, 0));
  treeColor = strip.gamma32(strip.Color(128, 255, 128));
  strip.fill(treeColor, 0, 84);
  strip.setBrightness(brightness);
  strip.show();

  // ceiling LED
//  pinMode(ledRpin, OUTPUT);
//  pinMode(ledGpin, OUTPUT);
//  pinMode(ledBpin, OUTPUT);
//  analogWrite(ledRpin, 0);
//  analogWrite(ledGpin, 0);
//  analogWrite(ledBpin, 0);
}

void loop() {
  //  if(Serial.available()) state = Serial.read();
  //  if (prestate != state) {
  //    stateChanged();
  //  }else{
  //  switch (state) {
  //    case 1:
  //      stateOne(); break;
  //    case 2:
  //      stateTwo(); break;
  //    case 3:
  //      stateThree();
  //      break;
  //    default:
  //      break;
  //  } prestate = state;
  //      stateOne();
  //  }
  //  if(Serial.available()) tempState = Serial.parseInt();
  //
  //  if (brightness>254 || brightness<2){
  //    brightAdd *= -1;
  //  }
  //  brightness += brightAdd;
  //  strip.setBrightness(brightness);
  //  strip.show();
  //  delay(10);
//    if(testFlag == 1){
//      wave();
//    }
  HueFlow();
}

void stateChanged() {
  if (brightness > 0) {
    --brightness;
  }
  strip.fill(background, 0, 84);
  strip.setBrightness(brightness);
  strip.show();

  if (Rvalue < 255) ++Rvalue;
  if (Gvalue < 255) ++Gvalue;
  if (Bvalue < 255) ++Bvalue;
  analogWrite(ledRpin, Rvalue);
  analogWrite(ledGpin, Gvalue);
  analogWrite(ledBpin, Bvalue);

  if (brightness <= 0 && Rvalue >= 255 && Gvalue >= 255 && Bvalue >= 255) {
    prestate = state;
  }
}


/*
   No one in the room.
   Floor LED breathing: white(255, 255, 255), 0%~50% brightness.
*/
void stateOne() {
  if (brightness > 127) {
    brightness = 1;
  }
  brightness += brightAdd;
  strip.fill(background, 0, 84);
  strip.setBrightness(calculateBrightnessToSin(brightness, 1.0, 128.0));
  strip.show();
  if (Rvalue < 255) ++Rvalue;
  if (Gvalue < 255) ++Gvalue;
  if (Bvalue < 255) ++Bvalue;
  analogWrite(ledRpin, Rvalue);
  analogWrite(ledGpin, Gvalue);
  analogWrite(ledBpin, Bvalue);
  delay(delayTime);
}


/*
   The door opened, someone enters the room.
   Floor LED: white(255, 255, 255), 100% brightness.
   Ceiling LED: white(255, 255, 255), 100% brightness.
*/
void stateTwo() {
  if (brightness < 255) {
    ++brightness;
  }
  strip.fill(background, 0, 84);
  strip.setBrightness(brightness);
  strip.show();

  if (prestate == 1) {
    analogWrite(ledRpin, 0);
    analogWrite(ledGpin, 0);
    analogWrite(ledBpin, 0);
  }
  else if (prestate == 3) {
    analogWrite(ledRpin, 255);
    analogWrite(ledGpin, 255);
    analogWrite(ledBpin, 255);
  }
  // play sound of turning on/off

  delay(delayTime);
}


/*
   Some one in room.
   Floor LED breathing and generate waves when moving
   interaction triggered
*/
void stateThree() {
  if (brightness > 254) {
    brightAdd = 127;
  }
  brightness ++;
  strip.setBrightness(calculateBrightnessToSin(brightness, 127.0, 255.0));
  strip.show();
  delay(delayTime);
}

/*
   Hue flow when looking at the painting
*/
void HueFlow() {
  for (int y = 0; y < 12; y++) {
    for (int x = 0; x < 7; x++) {
      strip.setPixelColor(cord2index(x, y), strip.gamma32(strip.ColorHSV(calculateBrightnessToSin(millis() * 4, -21844 - y * 920, 9557 - y * 920) + y * 920, 255, 255)));
    }
  }
  strip.show();
}

/*
   Generate wave when moving
*/
void wave() {
  //  if (millis()%500 == 0) testFlag = 0;
  for (int y = 0; y < 12; y++) {
    for (int x = 0; x < 7; x++) {
      radius = sqrt((x - 3) * (x - 3) + (y - 5.5) * (y - 5.5));
      thisColor = calculateBrightnessToLinear((radius * 2000 + millis() / 5), 1, 254);
      strip.setPixelColor(cord2index(x, y), thisColor, thisColor, thisColor);
    }
  }
  strip.show();
}

int cord2index(int x, int y) {
  if (x % 2 == 0) {
    return 12 * x + y;
  } else {
    return 12 * (x + 1) - 1 - y;
  }
}

float calculateBrightnessToSin(float brightness, float minBrightness, float maxBrightness) {
  return sin((brightness - minBrightness) / (maxBrightness - minBrightness) * 2 * PI) * (maxBrightness - minBrightness) / 2  + minBrightness / 2 + maxBrightness / 2;
}

float calculateBrightnessToLinear(int brightness, int minBrightness, int maxBrightness) {
  int thisColor = brightness % (2 * (maxBrightness - minBrightness));
  return abs(thisColor - (maxBrightness - minBrightness));
}

#include <Adafruit_NeoPixel.h>
//----------
// LED strip
//----------
#define LED_PIN    6
#define LED_COUNT 84
int brightness = 100;
int brightAdd = 1;
uint32_t background;
uint32_t bookColor;
uint32_t phoneColor;
uint32_t treeColor;
float radius = 0;
float thisColor = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  //Serial
  Serial.begin(9600);

  // LED strip
  strip.begin();
  background = strip.gamma32(strip.Color(255, 255, 255));
  bookColor = strip.gamma32(strip.Color(180, 146, 210));
  phoneColor = strip.gamma32(strip.Color(255, 0, 0));
  treeColor = strip.gamma32(strip.Color(128, 255, 128));
  strip.fill(background, 0, 84);
  strip.show();
}

void loop() {
    if (brightness >= 254 ) {
      brightAdd = -1;
      brightness = 254;
//      delay(3000);
    }
    if (brightness <= 2) {
      brightAdd = 1;
      brightness = 2;
//      delay(3000);
    }
    brightness += brightAdd;
    strip.setBrightness(brightness);
    Serial.println(brightness);
    //  strip.setBrightness(map(brightAdd, -abs(brightAdd), abs(brightAdd), 2, 254));
    strip.show();
    delay(20);
//    wave();
//    HueFlow();
}
/*
   Hue flow when looking at the painting
*/
void HueFlow() {
  for (int y = 0; y < 12; y++) {
    for (int x = 0; x < 7; x++) {
      strip.setPixelColor(cord2index(x, y), strip.gamma32(strip.ColorHSV(calculateBrightnessToSin(millis() * 4, -18164 - y * 920, 9557 - y * 920) + y * 920, 255, 255)));
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
      thisColor = calculateBrightnessToLinear((radius * 2000.0 + millis() / 5.0), 1, 254);
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

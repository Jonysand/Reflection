#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define LED_COUNT 50

int red = 250;
int green = 200;
int blue = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
}

void loop(){
  for(int i=0; i<LED_COUNT; ++i){
    strip.setPixelColor(i, ++green, ++red, 0);
    if(red>255) red=0;
    if(green>255) green=0;
    if(blue>255) blue=0;
    delay(5);
  }
  strip.show();
}

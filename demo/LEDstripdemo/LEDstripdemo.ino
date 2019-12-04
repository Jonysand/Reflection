#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define LED_COUNT 84

int pixelIndex = 0;
uint32_t plixelColor;
int brightness = 255;
int brightAdd = 1;
uint32_t background;
uint16_t HueValue = 256;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  //Serial
  Serial.begin(9600);
//  while (!Serial){}
  strip.begin();
  background = strip.Color(255, 255, 255);
  plixelColor = strip.Color(23, 161, 165);
  strip.setBrightness(255);
  strip.fill(background, 0, 84);
  strip.show();
}

void loop(){
  strip.fill(background, 0, pixelIndex);
  strip.fill(background, pixelIndex+1, 83-pixelIndex);
//  background = strip.ColorHSV(HueValue);
  strip.setPixelColor(pixelIndex, background);
  strip.setBrightness(brightness);
  strip.show();
//  brightness += brightAdd;
  if(brightness>254 || brightness<100){
    brightAdd *= -1;
  }
  HueValue += 10;
  if(HueValue>65535){
    HueValue = 0;
  }
  if(Serial.available()){
    pixelIndex = Serial.read();
  }
  delay(10);
}

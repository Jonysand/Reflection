#include <Adafruit_NeoPixel.h>

#define PIN 2

Adafruit_NeoPixel strip = Adafruit_NeoPixel(170, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
strip.begin();
strip.setBrightness(40);  // Lower brightness and save eyeballs!
strip.show(); // Initialize all pixels to 'off'
}


void loop() {

//(0, 152, 38, 69); // Redish
//  (1,184, 90, 107); // hazy pink
//  (2, 154, 77, 112); // between pink and purple
//  (3, 87, 43, 78);   //purple
//(4, 61, 88, 137);  //blue
// 
Wave(100); //calling the back the lower function
}


void Wave(uint8_t wait) {
int i, j, stripsize, cycle;
float ang, rsin, gsin, bsin, offset; //variable names

static int tick = 0;

stripsize = strip.numPixels();
cycle = stripsize * 20; // times around the circle...

while (++tick % cycle) {       //this is where things dint make sense no idea
  offset = map2PI(tick);

  for (i = 0; i < stripsize; i++) {
    ang = map2PI(i) - offset;
    rsin = sin(4.0 * ang / 5.0 + map2PI(int(stripsize/6)));  // purple  // color magic I kept playing with is until i got my two colors down
    //gsin = sin(4.0 * ang / 5.0 + map2PI(int(stripsize/6))); //green   //works like a color wheel
    bsin = sin(88); //blue
    strip.setPixelColor(i, strip.Color(trigScale(rsin), 0, trigScale(bsin)));
   
  }

  strip.show();
  delay(1);
}

}

/**
* Scale a value returned from a trig function to a byte value.
* [-1, +1] -> [0, 254] 
* brightness levels of 254 and 255.
*/
byte trigScale(float val) {
val += 1.0; // move range to [0.0, 2.0]
val *= 127.0; // move range to [0.0, 254.0]

return int(val) & 255;
}

/**
* Map an integer so that [0, striplength] -> [0, 2PI]
*/
float map2PI(int i) {
return PI*2.0*float(i) / float(strip.numPixels());
}

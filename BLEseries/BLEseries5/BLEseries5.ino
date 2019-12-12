/*
   Central
   controlling LED and 2 model chairs
*/

#include <ArduinoBLE.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

//----
// BLE
//----
byte data1 = 0;
byte data2 = 0;
byte data3 = 0;
byte data4 = 0;
byte data5 = 0;

//-----------
// State Flow
//-----------
byte state = 3;
byte prestate = 3;
int delayTime = 40;
int stateThreeBrightnessMin = 127;

//----------
// LED strip
//----------
#define LED_PIN    6
#define LED_COUNT 84
int pixelIndex = 42;
int PrepixelIndex = 42;
int brightness = 0;
uint32_t background;
uint32_t bookColor;
uint32_t phoneColor;
uint32_t treeColor;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);


//----------------
// ceiling LED
//----------------
#define ledRpin 12
#define ledGpin 11
#define ledBpin 10
int Rvalue = 255;
int Gvalue = 255;
int Bvalue = 255;

//--------------
// Other Control
//--------------
#define chairPin1 5
#define chairPin2 3
int chair1Flag = -1;
Servo chairServo1;
int chair2Flag = -1;
Servo chairServo2;
int waveFlag = -1; // when >0, start a wave

//-------------
// Serial setup
//-------------
const byte bookData = 64;
const byte phoneData = 128;
const byte lampData = 192;

void setup() {
  // Debug
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ledRpin, OUTPUT);
  pinMode(ledGpin, OUTPUT);
  pinMode(ledBpin, OUTPUT);
  digitalWrite(ledRpin, HIGH);
  digitalWrite(ledGpin, HIGH);
  digitalWrite(ledBpin, HIGH);

  // LED
  strip.begin();
  background = strip.gamma32(strip.Color(255, 255, 255));
  bookColor = strip.gamma32(strip.Color(223, 196, 255));
  phoneColor = strip.gamma32(strip.Color(255, 0, 0));
  treeColor = strip.gamma32(strip.Color(128, 255, 128));
  strip.fill(background, 0, 84);
  strip.show();

  // Other control
  chairServo1.attach(chairPin1);
  chairServo2.attach(chairPin2);

  // BLE
  Serial.begin (9600);
  BLE.begin();
  BLE.scanForUuid("c648e398-1122-11ea-8d71-362b9e155667");
}

void loop() {
  BLEDevice peripheral = BLE.available();
  if (peripheral) {
    BLE.stopScan();
    getData(peripheral);
    BLE.scanForUuid("c648e398-1122-11ea-8d71-362b9e155667");
  }
}


//----------------------------------
void getData(BLEDevice peripheral) {
  if (peripheral.connect()) {
  } else {
    return;
  }
  if (peripheral.discoverAttributes()) {
  } else {
    peripheral.disconnect();
    return;
  }

  BLECharacteristic preData1Characteristic = peripheral.characteristic("c648e399-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData2Characteristic = peripheral.characteristic("c648e400-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData3Characteristic = peripheral.characteristic("c648e401-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData4Characteristic = peripheral.characteristic("c648e402-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData5Characteristic = peripheral.characteristic("c648e403-1122-11ea-8d71-362b9e155667");
  if (!preData1Characteristic || !preData2Characteristic || !preData3Characteristic || !preData4Characteristic || !preData5Characteristic) {
    peripheral.disconnect();
    return;
  }
  while (peripheral.connected()) {
    //------------
    // Update Data
    //------------
    preData1Characteristic.readValue(data1);
    preData2Characteristic.readValue(data2);
    preData3Characteristic.readValue(data3);
    preData4Characteristic.readValue(data4);
    preData5Characteristic.readValue(data5);

    if (Serial.available()) {
      pixelIndex = Serial.read();
    }

    //------
    // State
    //------
    if (data3 != 0) { // the door opened
//      state = 2;
      digitalWrite(LED_BUILTIN, HIGH);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      //      if (pixelIndex != -1) {
      //        state = 3;
      //      } else {
      //        state = 1;
      //      }
    }
    switch (state) {
      case 1:
        stateOne(); break;
      case 2:
        stateTwo(); break;
      case 3:
        stateThree(); break;
      default:
        stateOne();
    }

    //-----------------
    // Walking reaction
    //-----------------
    if (pixelIndex != PrepixelIndex) waveFlag = 1;
    if (waveFlag > 0) wave();
  }
  //  }
  peripheral.disconnect();
}


//--------------------------------------------------------------
int cord2index(int x, int y) {
  if (x % 2 == 0) {
    return 12 * x + y;
  } else {
    return 12 * (x + 1) - 1 - y;
  }
}


/*-------------------------------------------------------------
   No one in the room.
   Floor LED breathing: white(255, 255, 255), 0%~50% brightness.
*/
void stateOne() {
  if (brightness > 127) {
    brightness = 1;
  }
  ++ brightness;
  strip.fill(background, 0, 84);
  strip.setBrightness(calculateBrightnessToSin(brightness, 1.0, 128.0));
  strip.show();
  if (Rvalue < 255) ++Rvalue;
  if (Gvalue < 255) ++Gvalue;
  if (Bvalue < 255) ++Bvalue;
  analogWrite(ledRpin, Rvalue);
  analogWrite(ledGpin, Gvalue);
  analogWrite(ledBpin, Bvalue);
}


/*-------------------------------------------------------------
   The door opened, someone enters the room.
   Floor LED: white(255, 255, 255), 100% brightness.
*/
void stateTwo() {
  if (brightness < 254) {
    ++brightness;
  }
  strip.fill(background, 0, 84);
  strip.setBrightness(brightness);
  strip.show();
  // play sound of turning on/off
  //  if (pixelIndex != -1) {
  //    state = 3;
  //    prestate = state;
  //    analogWrite(ledRpin, 0);
  //    analogWrite(ledGpin, 0);
  //    analogWrite(ledBpin, 0);
  //  } else {
  //    state = 1;
  //    prestate = state;
  //    analogWrite(ledRpin, 255);
  //    analogWrite(ledGpin, 255);
  //    analogWrite(ledBpin, 255);
  //  }
  digitalWrite(ledRpin, LOW);
  digitalWrite(ledGpin, LOW);
  digitalWrite(ledBpin, LOW);
  state = 3;
}


/*-------------------------------------------------------------
   Some one in room.
   Floor LED breathing and generate waves when moving
   interaction triggered
*/

void stateThree() {
  if (brightness >= 512) {
    brightness = 0;
  }
  ++brightness;

  strip.setBrightness(calculateBrightnessToLinear(brightness, stateThreeBrightnessMin, 254.0));
  strip.show();

  //------
  // Phone & Book
  //------
  if (data4 == 255) { // Phone
    background = phoneColor;
  }
  if (data5 == 255) { // Book
    background = bookColor;
  } else if (data4 != 255 && data5 != 255) {
    background = strip.gamma32(strip.Color(255, 255, 255));
  }

  //--------------
  // Near the tree
  //--------------
  if (pixelIndex == 82 || pixelIndex == 81 || pixelIndex == 83 || pixelIndex == 60 || pixelIndex == 61 || pixelIndex == 62 || pixelIndex == 59 || pixelIndex == 58 || pixelIndex == 57) {
    background = treeColor;
  }
  //--------------
  //Near the paint
  //--------------
  if (pixelIndex == 0 || pixelIndex == 1 || pixelIndex == 2 || pixelIndex == 23 || pixelIndex == 22 || pixelIndex == 21 || pixelIndex == 24 || pixelIndex == 25 || pixelIndex == 26) {
    HueFlow();
  } else {
    strip.fill(background, 0, 84);
  }

  //--------
  // Chair 1
  //--------
  if (data1 == 255) {
    if (chair1Flag < 0 ) chairServo1.write(60);
    else chairServo1.write(0);
    chair1Flag *= -1;
  } else {
    chairServo1.write(0);
  }

  //--------
  // Chair 2
  //--------
  if (data2 == 255) {
    if (chair2Flag < 0 ) chairServo2.write(120);
    else chairServo2.write(0);
    chair2Flag *= -1;
  } else {
    chairServo2.write(0);
  }
}

/*
   Hue flow when looking at the painting
*/
void HueFlow() {
  for (int y = 0; y < 12; y++) {
    for (int x = 0; x < 7; x++) {
      strip.setPixelColor(cord2index(x, y), strip.gamma32(strip.ColorHSV(calculateBrightnessToSin(millis() * 2, -23666 - y * 920, 9557 - y * 920) + y * 920, 255, 255)));
    }
  }
  strip.show();
}

/*
   Generate wave when moving
*/
int radius = 0;
float thisColor = 0;
void wave() {
  if (brightness % 10 == 0) {
    waveFlag = -1;
    PrepixelIndex = pixelIndex;
  }
  for (int y = 0; y < 12; y++) {
    for (int x = 0; x < 7; x++) {
      radius = sqrt((x - 3) * (x - 3) + (y - 5.5) * (y - 5.5));
      thisColor = calculateBrightnessToLinear((radius * 2000 + millis() / 5), 1, 254);
      strip.setPixelColor(cord2index(x, y), thisColor, thisColor, thisColor);
    }
  }
  strip.show();
}

float calculateBrightnessToSin(float brightness, float minBrightness, float maxBrightness) {
  return sin((brightness - minBrightness) / (maxBrightness - minBrightness) * 2 * PI) * (maxBrightness - minBrightness) / 2  + minBrightness / 2 + maxBrightness / 2;
}

float calculateBrightnessToLinear(int brightness, int minBrightness, int maxBrightness) {
  thisColor = brightness % (2 * (maxBrightness - minBrightness));
  return abs(thisColor - (maxBrightness - minBrightness)+minBrightness);
}

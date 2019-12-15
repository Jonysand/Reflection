/*
   Central
*/

#include <ArduinoBLE.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

//---------------
// Timing Control
//---------------
byte loopClock = 0;

//----
// BLE
//----
byte data1 = 0;
byte data2 = 0;
byte data3 = 0;
byte data4 = 0;
byte data5 = 0;
byte dataReturned = 0;

//----------
// LED strip
//----------
#define LED_PIN    6
#define LED_COUNT 84
int serialData = 0;
int minBrightness = 50;
int maxBrightness = 254;
int brightness = minBrightness + 1;
int brightnessAdd = 2;
uint32_t background;
uint32_t bookColor;
uint32_t phoneColor;
uint32_t treeColor;
uint32_t defaultColor;
bool isHueFlow = false;
bool isPhone = false;
bool isBook = false;
bool isTree = false;
bool doorOpened = false;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);


//----------------
// ceiling LED
//----------------
#define ledRpin 12
#define ledGpin 11
#define ledBpin 10

//--------------
// Chair Control
//--------------
#define chairPin1 5
#define chairPin2 3
int chair1Flag = -1;
Servo chairServo1;
int chair2Flag = -1;
Servo chairServo2;

//-------------
// Serial setup
//-------------
const byte nullData = 1;
const byte treeData = 32;
const byte bookData = 64;
const byte phoneData = 128;
const byte lampData = 192;
const byte paintData = 224;



void setup() {
  // Debug
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // Ceiling LED
  analogWrite(ledRpin, 255);
  analogWrite(ledGpin, 255);
  analogWrite(ledBpin, 255);

  // LED Strip
  strip.begin();
  bookColor = strip.gamma32(strip.Color(180, 146, 210));
  phoneColor = strip.gamma32(strip.Color(255, 0, 0));
  treeColor = strip.gamma32(strip.Color(128, 255, 128));
  defaultColor = strip.gamma32(strip.Color(255, 255, 255));
  background = defaultColor;
  strip.fill(background, 0, 84);
  strip.show();

  // Chair control
  chairServo1.attach(chairPin1);
  chairServo1.write(0);
  chairServo2.attach(chairPin2);
  chairServo2.write(0);

  // BLE
  BLE.begin();
  BLE.scanForUuid("c648e398-1122-11ea-8d71-362b9e155667");
}

void loop() {
  BLEDevice peripheral = BLE.available();
  if (peripheral) {
    BLE.stopScan();
    DeviceLoop(peripheral);
    BLE.scanForUuid("c648e398-1122-11ea-8d71-362b9e155667");
  }
}



//----------------------------------
void DeviceLoop(BLEDevice peripheral) {
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
    switch (loopClock % 5 + 1) {
      case 1:
        preData1Characteristic.readValue(dataReturned);
        data1 = dataReturned;
        break;
      case 2:
        preData2Characteristic.readValue(dataReturned);
        data2 = dataReturned;
        break;
      case 3:
        preData3Characteristic.readValue(dataReturned);
        data3 = dataReturned;
        break;
      case 4:
        preData4Characteristic.readValue(dataReturned);
        data4 = dataReturned;
        break;
      case 5:
        preData5Characteristic.readValue(dataReturned);
        data5 = dataReturned;
        break;
      default:
        break;
    }
    if (Serial.available()) {
      serialData = Serial.read();
    }

    ++loopClock;
    if (loopClock >= 255) loopClock = 0;
    ControlLoop();
  }

  peripheral.disconnect();
}



//--------------------------------------------------------------
void ControlLoop() {
  //----------------------
  // Reset LED Floor Color
  //----------------------
  if (serialData == nullData) {
    isHueFlow = false;
    isTree = false;
  }
  if (!isPhone && !isBook && !isTree) {
    background = defaultColor;
  }
  if (!isPhone && !isTree && data3 != 255 && !isBook) {
    if (Serial.availableForWrite()) Serial.write(nullData);
  }

  //-----
  // Door
  //-----
  if (data3 == 255) { // the door opened
    analogWrite(ledRpin, 0);
    analogWrite(ledGpin, 0);
    analogWrite(ledBpin, 0);
    if (Serial.availableForWrite() && !doorOpened) {
      Serial.write(lampData);
      doorOpened = true;
    }
  }else{
    analogWrite(ledRpin, 255);
    analogWrite(ledGpin, 255);
    analogWrite(ledBpin, 255);
    doorOpened = false;
  }

  //-----
  // Book
  //-----
  if (data5 == 255) {
    if (!isBook) {
      brightnessAdd = -abs(brightnessAdd);
      minBrightness = 20;
      if ((brightness + brightnessAdd) <= minBrightness) {
        background = bookColor;
        isBook = true;
        if (Serial.availableForWrite()) Serial.write(bookData);
      }
    }
  } else {
    isBook = false;
    minBrightness = 50;
    brightness = constrain(brightness, minBrightness, maxBrightness);
  }

  //--------------
  // Near the tree
  //--------------
  if (serialData == treeData) {
    if (!isTree) {
      brightnessAdd = -abs(brightnessAdd);
      if ((brightness + brightnessAdd) <= minBrightness) {
        isTree = true;
        background = treeColor;
      }
    }
  }

  //---------------
  // Near the paint
  //---------------
  if (serialData == paintData) {
    if (!isHueFlow) {
      brightnessAdd = -abs(brightnessAdd);
      if ((brightness + brightnessAdd) <= minBrightness) {
        isHueFlow = true;
      }
    }
  }

  //------
  // Phone
  //------
  if (data4 == 255) {
    if (!isPhone) {
      brightnessAdd = -abs(brightnessAdd);
      if ((brightness + brightnessAdd) <= minBrightness) {
        background = phoneColor;
        isPhone = true;
        if (Serial.availableForWrite()) Serial.write(phoneData);
      }
    }
  } else {
    isPhone = false;
  }

  //--------
  // Chair 1
  //--------
  if (data1 == 255) {
    if (loopClock % 10 == 1) {
      if (chair1Flag < 0 ) chairServo1.write(60);
      else chairServo1.write(0);
      chair1Flag *= -1;
    }
  } else {
    chairServo1.write(0);
  }

  //--------
  // Chair 2
  //--------
  if (data2 == 255) {
    if (loopClock % 10 == 1) {
      if (chair2Flag < 0 ) chairServo2.write(120);
      else chairServo2.write(0);
      chair2Flag *= -1;
    }
  } else {
    chairServo2.write(0);
  }

  //-----------------
  // LED strip update
  //-----------------
  if (isHueFlow) HueFlow();
  else strip.fill(background, 0, 84);
  if (brightness >= maxBrightness) {
    brightnessAdd = -abs(brightnessAdd);
  } else if (brightness <= minBrightness) {
    brightnessAdd = abs(brightnessAdd);
  }
  brightness += brightnessAdd;
  if (isPhone) strip.setBrightness(map(brightnessAdd, -abs(brightnessAdd), abs(brightnessAdd), 0, 254));
  else strip.setBrightness(brightness);
  strip.show();
}


//--------------------------------------------------------------
int cord2index(int x, int y) {
  if (x % 2 == 0) {
    return 12 * x + y;
  } else {
    return 12 * (x + 1) - 1 - y;
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
}

float calculateBrightnessToSin(float brightness, float minBrightness, float maxBrightness) {
  return sin((brightness - minBrightness) / (maxBrightness - minBrightness) * 2 * PI) * (maxBrightness - minBrightness) / 2  + minBrightness / 2 + maxBrightness / 2;
}

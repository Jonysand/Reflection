/*
 * Central
 * controlling LED and 2 model chairs
 */

#include <ArduinoBLE.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define LED_COUNT 84
#define chairPin1 3
#define chairPin2 5
#define ledRpin 12
#define ledGpin 11
#define ledBpin 10


//----
// BLE
//----
byte data1 = 0;
byte data2 = 0;
byte data3 = 0;
byte data4 = 0;
byte data5 = 0;

//----------
// LED strip
//----------
int pixelIndex = 0;
uint32_t plixelColor;
int brightness = 100;
int brightAdd = 1;
uint32_t background;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

//--------------
// Other Control
//--------------
Servo chairServo1;
Servo chairServo2;


void setup() {
  // LED
  strip.begin();
  background = strip.Color(100, 100, 100);
  plixelColor = strip.Color(0, 0, 255);
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
  if (peripheral){
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
  while(peripheral.connected()){
    preData1Characteristic.readValue(data1);
    preData2Characteristic.readValue(data2);
    preData3Characteristic.readValue(data3);
    preData4Characteristic.readValue(data4);
    preData5Characteristic.readValue(data5);

    // LED
    strip.fill(background, 0, pixelIndex);
    strip.fill(background, pixelIndex+1, 83-pixelIndex);
    strip.setPixelColor(pixelIndex, plixelColor);
    strip.setBrightness(brightness);
    strip.show();
    brightness += brightAdd;
    if(brightness>254 || brightness<100){
      brightAdd *= -1;
    }
    if(Serial.available()){
      pixelIndex = Serial.read();
    }

    //--------
    // Chair 1
    //--------
    if(data1 == 255){
      chairServo1.write(90);
    }else{
      chairServo1.write(0);
    }

    //--------
    // Chair 2
    //--------
    if(data2 == 255){
      chairServo2.write(90);
    }else{
      chairServo2.write(0);
    }

    //-----
    // Door
    //-----
    if(data3 == 255){
      analogWrite(ledRpin, 0);
      analogWrite(ledGpin, 0);
      analogWrite(ledBpin, 0);
    }else{
      analogWrite(ledRpin, 255 - lowByte(background>>16));
      analogWrite(ledGpin, 255 - lowByte(background>>8));
      analogWrite(ledBpin, 255 - lowByte(background));
    }

    //------
    // Phone & Book
    //------
    if(data4 == 255){
      background = strip.Color(100, 0, 0);
    }
    if(data5 == 255){
      background = strip.Color(0, 100, 0);
    }
    if (data4 != 255 & data5 != 255) {
      background = strip.Color(100, 100, 100);
    }
  }
  
  peripheral.disconnect();
}

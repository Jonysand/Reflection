#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define LED_COUNT 84

//----
// BLE
//----
byte data1 = 0;
byte data2 = 0;
byte data3 = 0;
byte data4 = 0;
byte data5 = 100;

//----------
// LED strip
//----------
int pixelIndex = 0;
uint32_t plixelColor;
int brightness = 100;
int brightAdd = 1;
uint32_t background;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  // LED
  strip.begin();
  background = strip.Color(0, 0, 0);
  plixelColor = strip.Color(0, 20, 20);
  strip.fill(background, 0, 84);
  strip.show();

  // BLE
  Serial.begin (9600);
  BLE.begin();
  Serial.println("BLE series 5 ready!");
  BLE.scanForUuid("c648e398-1122-11ea-8d71-362b9e155667");
}

void loop() {
  BLEDevice peripheral = BLE.available();
  if (peripheral){
    Serial.println("Found");
    BLE.stopScan();
    getData(peripheral);
    BLE.scanForUuid("c648e398-1122-11ea-8d71-362b9e155667");
  }
}


//----------------------------------
void getData(BLEDevice peripheral) {
  if (peripheral.connect()) {
  } else {
    Serial.println("fail connect!");
    return;
  }
  if (peripheral.discoverAttributes()) {
  } else {
    Serial.println("fail discoverAttributes!");
    peripheral.disconnect();
    return;
  }

  BLECharacteristic preData1Characteristic = peripheral.characteristic("c648e399-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData2Characteristic = peripheral.characteristic("c648e400-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData3Characteristic = peripheral.characteristic("c648e401-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData4Characteristic = peripheral.characteristic("c648e402-1122-11ea-8d71-362b9e155667");
  if (!preData1Characteristic || !preData2Characteristic || !preData3Characteristic || !preData4Characteristic) {
    Serial.println("fail BLECharacteristic!");
    peripheral.disconnect();
    return;
  }
  while(peripheral.connected()){
    preData1Characteristic.readValue(data1);
    preData2Characteristic.readValue(data2);
    preData3Characteristic.readValue(data3);
    preData4Characteristic.readValue(data4);

    if(data1 == 100){
      strip.fill(plixelColor, 0, 12);
    }else{
      strip.fill(background, 0, 12);
    }
    
    if(data2 == 100){
      strip.fill(plixelColor, 12, 12);
    }else{
      strip.fill(background, 12, 12);
    }
    
    if(data3 == 100){
      strip.fill(plixelColor, 24, 12);
    }else{
      strip.fill(background, 24, 12);
    }
    
    if(data4 == 100){
      strip.fill(plixelColor, 36, 12);
    }else{
      strip.fill(background, 36, 12);
    }
    
    if(data5 == 100){
      strip.fill(plixelColor, 48, 12);
    }else{
      strip.fill(background, 48, 12);
    }
    
    strip.show();
  }
  
  peripheral.disconnect();
}

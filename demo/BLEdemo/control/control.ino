#include <ArduinoBLE.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define LED_COUNT 84

int nBytesRead;
byte data;
byte dataReturned;
uint32_t background;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);

  //----------
  // LED setup
  //----------
  strip.begin();
  background = strip.Color(181, 200, 230);
  strip.fill(background, 0, 84);
  strip.show();

  //---------
  //BLE setup
  //---------
  BLE.begin();
  Serial.println("BLE Central");
  // start scanning for peripherals
  BLE.scanForUuid("ede2f3d6-856d-41a8-811b-ed1385c2d44a");
}

void loop() {
  BLEDevice peripheral = BLE.available();
  if (peripheral) {
    BLE.stopScan();
    getData(peripheral);
    BLE.scanForUuid("ede2f3d6-856d-41a8-811b-ed1385c2d44a");
  }
}

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

  BLECharacteristic chairCharacteristic = peripheral.characteristic("ede2f3d7-856d-41a8-811b-ed1385c2d44a");
  chairCharacteristic.subscribe();
  if (!chairCharacteristic) {
    peripheral.disconnect();
    return;
  }
  
  while (peripheral.connected()) {
    if (chairCharacteristic.valueUpdated()) {
      chairCharacteristic.readValue(dataReturned);
      data = dataReturned;
    }
    if(data==255) digitalWrite(2, HIGH);
    else digitalWrite(2, LOW);
//    Serial.println(data);
    
  }

  peripheral.disconnect();
}

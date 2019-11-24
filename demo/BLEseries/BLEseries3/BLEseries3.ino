/*
 * This device just reads data.
 */

#include <ArduinoBLE.h>

#define Rpin 2
#define Gpin 3
#define Bpin 4

byte redValue = 0;
byte greenValue = 0;
byte blueValue = 100;

void setup() {
  Serial.begin (9600);
  BLE.begin();
  Serial.println("BLE series 3 ready!");
  BLE.scanForUuid("cea9da68-0ee5-11ea-8d71-362b9e155667");
}

void loop() {
  BLEDevice peripheral = BLE.available();
  if (peripheral){
    Serial.println("Found");
    BLE.stopScan();
    getData(peripheral);
    BLE.scanForUuid("cea9da68-0ee5-11ea-8d71-362b9e155667");
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

  BLECharacteristic redCharacteristic = peripheral.characteristic("cea9da69-0ee5-11ea-8d71-362b9e155667");
  BLECharacteristic greenCharacteristic = peripheral.characteristic("cea9da70-0ee5-11ea-8d71-362b9e155667");
  if (!redCharacteristic|| !greenCharacteristic) {
    Serial.println("fail BLECharacteristic!");
    peripheral.disconnect();
    return;
  }
  while(peripheral.connected()){
    redCharacteristic.readValue(redValue);
    greenCharacteristic.readValue(greenValue);

    analogWrite(Rpin, redValue);
    analogWrite(Gpin, greenValue);
    analogWrite(Bpin, blueValue);
  }
  peripheral.disconnect();
}

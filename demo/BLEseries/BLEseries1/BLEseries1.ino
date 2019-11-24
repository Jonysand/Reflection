/* 
 * This device just advertises sensor data.
 */

#include <ArduinoBLE.h>

#define Rpin 2
#define Gpin 3
#define Bpin 4

byte redValue = 100;
byte greenValue = 0;
byte blueValue = 0;

BLEService firstService("cea9d806-0ee5-11ea-8d71-362b9e155667");
BLEByteCharacteristic redCharacteristic("cea9d807-0ee5-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin (9600);

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("BLEseries1");
  BLE.setAdvertisedService(firstService);
  firstService.addCharacteristic(redCharacteristic);
  BLE.addService(firstService);
  redCharacteristic.writeValue(redValue);
  BLE.advertise();

  Serial.println("BLE series 1 ready!");
}

void loop() {
  //-----------------------------
  // search for downstream client
  //-----------------------------
  BLEDevice central = BLE.central();
  if (central) {
    Serial.println(central.address());
    while (central.connected()){
      
    }
  }
}

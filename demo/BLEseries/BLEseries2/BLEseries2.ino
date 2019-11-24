/*
 * This device both advertises sensor data,
 * and reads data from other devices.
 */

#include <ArduinoBLE.h>

#define Rpin 2
#define Gpin 3
#define Bpin 4

byte redValue = 0;
byte greenValue = 100;
byte blueValue = 0;

BLEService sencondService("cea9da68-0ee5-11ea-8d71-362b9e155667");
BLEByteCharacteristic redCharacteristic("cea9da69-0ee5-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic greenCharacteristic("cea9da70-0ee5-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin (9600);

  BLE.begin();

  BLE.setLocalName("BLEseries2");
  BLE.setAdvertisedService(sencondService);
  sencondService.addCharacteristic(redCharacteristic);
  sencondService.addCharacteristic(greenCharacteristic);
  BLE.addService(sencondService);
  redCharacteristic.writeValue(redValue);
  greenCharacteristic.writeValue(greenValue);
  BLE.advertise();

  Serial.println("BLE series 2 ready!");
  BLE.scanForUuid("cea9d806-0ee5-11ea-8d71-362b9e155667");
}

void loop() {
  //-----------------------------
  // search for downstream client
  //-----------------------------

  BLEDevice peripheral = BLE.available();
  if (peripheral){
    Serial.println("Found");
    BLE.stopScan();
    getData(peripheral);
    BLE.scanForUuid("cea9d806-0ee5-11ea-8d71-362b9e155667");
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

  BLECharacteristic redfromupCharacteristic = peripheral.characteristic("cea9d807-0ee5-11ea-8d71-362b9e155667");
  if (!redfromupCharacteristic) {
    Serial.println("fail BLECharacteristic!");
    peripheral.disconnect();
    return;
  }
  while(peripheral.connected()){
    redfromupCharacteristic.readValue(redValue);
    redCharacteristic.writeValue(redValue);
    analogWrite(Rpin, redValue);
    analogWrite(Gpin, greenValue);
    analogWrite(Bpin, blueValue);

    BLEDevice central = BLE.central();
    if (central){
      Serial.println(central.address());
      while (central.connected()){
        
      }
    }
  }
  
  peripheral.disconnect();
}

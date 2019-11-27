/*
 * This device both advertises sensor data,
 * and reads data from other devices.
 */

#include <ArduinoBLE.h>

#define LEDpin 2

byte data1 = 0;
byte data2 = 0;
byte data3 = 100;

BLEService thirdService("c648e26c-1122-11ea-8d71-362b9e155667");
BLEByteCharacteristic data1Characteristic("c648e26d-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic data2Characteristic("c648e26e-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic data3Characteristic("c648e26f-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin (9600);
  pinMode(LEDpin, OUTPUT);

  BLE.begin();
  BLE.setLocalName("BLEseries3");
  BLE.setAdvertisedService(thirdService);
  thirdService.addCharacteristic(data1Characteristic);
  thirdService.addCharacteristic(data2Characteristic);
  thirdService.addCharacteristic(data3Characteristic);
  BLE.addService(thirdService);
  data1Characteristic.writeValue(data1);
  data2Characteristic.writeValue(data2);
  data3Characteristic.writeValue(data3);
  BLE.advertise();

  Serial.println("BLE series 3 ready!");
  BLE.scanForUuid("c648e0dc-1122-11ea-8d71-362b9e155667");
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
    BLE.scanForUuid("c648e0dc-1122-11ea-8d71-362b9e155667");
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

  BLECharacteristic preData1Characteristic = peripheral.characteristic("c648e0dd-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData2Characteristic = peripheral.characteristic("c648e0de-1122-11ea-8d71-362b9e155667");
  if (!preData1Characteristic || !preData2Characteristic) {
    Serial.println("fail BLECharacteristic!");
    peripheral.disconnect();
    return;
  }
  while(peripheral.connected()){
    BLEDevice central = BLE.central();
    if (central){
      Serial.println(central.address());
      while (central.connected()){
        digitalWrite(LEDpin, HIGH);
        preData1Characteristic.readValue(data1);
        preData2Characteristic.readValue(data2);
        data1Characteristic.writeValue(data1);
        data2Characteristic.writeValue(data2);
      }
      digitalWrite(LEDpin, LOW);
    }
  }
  
  peripheral.disconnect();
}

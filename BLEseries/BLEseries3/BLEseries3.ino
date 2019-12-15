/*
   Door

   data3Characteristic:
   0 -> closed;
   255 -> opened;
*/

#include <ArduinoBLE.h>

#define doorPin 2

int doorState = LOW;

byte data1 = 0;
byte data2 = 0;
byte data3 = 0;
byte dataReturned;

BLEService thirdService("c648e26c-1122-11ea-8d71-362b9e155667");
BLEByteCharacteristic data1Characteristic("c648e26d-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLEIndicate);
BLEByteCharacteristic data2Characteristic("c648e26e-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLEIndicate);
BLEByteCharacteristic data3Characteristic("c648e26f-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLEIndicate);
BLECharacteristic preData1Characteristic;
BLECharacteristic preData2Characteristic;

void setup() {
  Serial.begin (9600);
  pinMode(LED_BUILTIN , OUTPUT);
  pinMode(doorPin, INPUT);

  BLE.begin();
  BLE.setLocalName("BLE_door_<3>");
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
  if (peripheral) {
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

  preData1Characteristic = peripheral.characteristic("c648e0dd-1122-11ea-8d71-362b9e155667");
  preData2Characteristic = peripheral.characteristic("c648e0de-1122-11ea-8d71-362b9e155667");
  preData1Characteristic.setEventHandler(BLEWritten, data1Written);
  preData2Characteristic.setEventHandler(BLEWritten, data2Written);
  if (!preData1Characteristic || !preData2Characteristic) {
    Serial.println("fail BLECharacteristic!");
    peripheral.disconnect();
    return;
  }
  preData1Characteristic.subscribe();
  preData2Characteristic.subscribe();
  while (peripheral.connected()) {
    BLEDevice central = BLE.central();
    if (central) {
      Serial.println(central.address());
      while (central.connected()) {
        digitalWrite(LED_BUILTIN , HIGH);

        if (digitalRead(doorPin) == LOW) {
          if (data3Characteristic.value() == 0)
            data3Characteristic.writeValue(255);
        } else {
          if (data3Characteristic.value() == 255)
            data3Characteristic.writeValue(0);
        }
      }
      digitalWrite(LED_BUILTIN , LOW);
    }
  }

  peripheral.disconnect();
}

void data1Written(BLEDevice central, BLECharacteristic characteristic) {
  preData1Characteristic.readValue(dataReturned);
  if (data1 != dataReturned) {
    data1 = dataReturned;
    data1Characteristic.writeValue(data1);
  }
}

void data2Written(BLEDevice central, BLECharacteristic characteristic) {
  preData2Characteristic.readValue(dataReturned);
  if (data2 != dataReturned) {
    data2 = dataReturned;
    data2Characteristic.writeValue(data2);
  }
}

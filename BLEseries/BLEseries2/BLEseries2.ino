/*
   Chair 2

   data2Characteristic:
   0 -> not sitted on;
   255 -> sitted on;
*/

#include <ArduinoBLE.h>

#define seatPin A7

const byte howMany = 100;
int readings[howMany];
int readIndex = 0;
int total = 0;
int serialCount = 0;
int sensorValue = 0;

byte data1 = 0;
byte data2 = 0;
byte dataReturned;

BLEService sencondService("c648e0dc-1122-11ea-8d71-362b9e155667");
BLEByteCharacteristic data1Characteristic("c648e0dd-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLEIndicate);
BLEByteCharacteristic data2Characteristic("c648e0de-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLEIndicate);

BLECharacteristic preData1Characteristic;

void setup() {
  Serial.begin (9600);
  pinMode(seatPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  BLE.begin();
  BLE.setLocalName("BLE_chair2_<2>");
  BLE.setAdvertisedService(sencondService);
  sencondService.addCharacteristic(data1Characteristic);
  sencondService.addCharacteristic(data2Characteristic);
  BLE.addService(sencondService);
  data1Characteristic.writeValue(data1);
  data2Characteristic.writeValue(data2);
  BLE.advertise();

  Serial.println("BLE series 2 ready!");
  BLE.scanForUuid("c648dc40-1122-11ea-8d71-362b9e155667");
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
    BLE.scanForUuid("c648dc40-1122-11ea-8d71-362b9e155667");
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

  preData1Characteristic = peripheral.characteristic("c648dc41-1122-11ea-8d71-362b9e155667");
  preData1Characteristic.setEventHandler(BLEWritten, data1Written);
  if (!preData1Characteristic) {
    Serial.println("fail BLECharacteristic!");
    peripheral.disconnect();
    return;
  }
  preData1Characteristic.subscribe();
  while (peripheral.connected()) {
    BLEDevice central = BLE.central();
    if (central) {
      while (central.connected()) {
        digitalWrite(LED_BUILTIN, HIGH);

        total -= readings[readIndex];
        readings[readIndex] = analogRead(A7);
        total += readings[readIndex];
        readIndex++;
        if (readIndex >= howMany) {
          readIndex = 0;
        }
        sensorValue = total / howMany;
        if (sensorValue < 1000) {
          if (data2Characteristic.value() == 0)
            data2Characteristic.writeValue(255);
        }
        else {
          if (data2Characteristic.value() == 255)
            data2Characteristic.writeValue(0);
        }
      }
      digitalWrite(LED_BUILTIN, LOW);
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

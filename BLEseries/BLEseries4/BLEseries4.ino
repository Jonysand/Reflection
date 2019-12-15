/*
   Phone, Book

   data4Characteristic:
   0 -> Phone on the table;
   255 -> Phone picked up;

   data5Characteristic:
   0 -> Book on the table;
   255 -> Book picked up;
*/

#include <ArduinoBLE.h>

#define phonePin A6
#define bookPin A7

byte data1 = 0;
byte data2 = 0;
byte data3 = 0;
byte data4 = 0;
byte data5 = 0;
byte dataReturned;

BLEService fourthService("c648e398-1122-11ea-8d71-362b9e155667");
BLEByteCharacteristic data1Characteristic("c648e399-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite);
BLEByteCharacteristic data2Characteristic("c648e400-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite);
BLEByteCharacteristic data3Characteristic("c648e401-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite);
BLEByteCharacteristic data4Characteristic("c648e402-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite);
BLEByteCharacteristic data5Characteristic("c648e403-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite);

BLECharacteristic preData1Characteristic;
BLECharacteristic preData2Characteristic;
BLECharacteristic preData3Characteristic;

void setup() {
  Serial.begin (9600);
  pinMode(LED_BUILTIN , OUTPUT);
  pinMode(phonePin, INPUT);
  pinMode(bookPin, INPUT);

  BLE.begin();
  BLE.setLocalName("BLE_P&B_<4>");
  BLE.setAdvertisedService(fourthService);
  fourthService.addCharacteristic(data1Characteristic);
  fourthService.addCharacteristic(data2Characteristic);
  fourthService.addCharacteristic(data3Characteristic);
  fourthService.addCharacteristic(data4Characteristic);
  fourthService.addCharacteristic(data5Characteristic);
  BLE.addService(fourthService);
  data1Characteristic.writeValue(data1);
  data2Characteristic.writeValue(data2);
  data3Characteristic.writeValue(data3);
  data4Characteristic.writeValue(data4);
  data5Characteristic.writeValue(data5);
  BLE.advertise();

  Serial.println("BLE series 4 ready!");
  BLE.scanForUuid("c648e26c-1122-11ea-8d71-362b9e155667");
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
    BLE.scanForUuid("c648e26c-1122-11ea-8d71-362b9e155667");
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

  preData1Characteristic = peripheral.characteristic("c648e26d-1122-11ea-8d71-362b9e155667");
  preData2Characteristic = peripheral.characteristic("c648e26e-1122-11ea-8d71-362b9e155667");
  preData3Characteristic = peripheral.characteristic("c648e26f-1122-11ea-8d71-362b9e155667");
  preData1Characteristic.setEventHandler(BLEWritten, data1Written);
  preData2Characteristic.setEventHandler(BLEWritten, data2Written);
  preData3Characteristic.setEventHandler(BLEWritten, data3Written);
  if (!preData1Characteristic || !preData2Characteristic || !preData3Characteristic) {
    Serial.println("fail BLECharacteristic!");
    peripheral.disconnect();
    return;
  }
  preData1Characteristic.subscribe();
  preData2Characteristic.subscribe();
  preData3Characteristic.subscribe();
  while (peripheral.connected()) {
    BLEDevice central = BLE.central();
    if (central) {
      Serial.println(central.address());
      while (central.connected()) {
        digitalWrite(LED_BUILTIN , HIGH);
        
        if (analogRead(phonePin) > 900) {
          if (data4Characteristic.value() == 255)
            data4Characteristic.writeValue(0);
        } else {
          if (data4Characteristic.value() == 0)
            data4Characteristic.writeValue(255);
        }

        if (analogRead(bookPin) > 900) {
          if (data5Characteristic.value() == 255)
            data5Characteristic.writeValue(0);
        } else {
          if (data5Characteristic.value() == 0)
            data5Characteristic.writeValue(255);
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

void data3Written(BLEDevice central, BLECharacteristic characteristic) {
  preData3Characteristic.readValue(dataReturned);
  if (data3 != dataReturned) {
    data3 = dataReturned;
    data3Characteristic.writeValue(data3);
  }
}

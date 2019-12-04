/*
 * Phone, Book
 * 
 * data4Characteristic:
 * 0 -> Phone on the table;
 * 255 -> Phone picked up;
 * 
 * data5Characteristic:
 * 0 -> Book on the table;
 * 255 -> Book picked up;
 */

#include <ArduinoBLE.h>

#define LEDpin 2
#define phonePin A7
#define bookPin A6

byte data1 = 0;
byte data2 = 0;
byte data3 = 0;
byte data4 = 255;
byte data5 = 255;

BLEService fourthService("c648e398-1122-11ea-8d71-362b9e155667");
BLEByteCharacteristic data1Characteristic("c648e399-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic data2Characteristic("c648e400-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic data3Characteristic("c648e401-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic data4Characteristic("c648e402-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic data5Characteristic("c648e403-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin (9600);
  pinMode(LEDpin, OUTPUT);
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
  if (peripheral){
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

  BLECharacteristic preData1Characteristic = peripheral.characteristic("c648e26d-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData2Characteristic = peripheral.characteristic("c648e26e-1122-11ea-8d71-362b9e155667");
  BLECharacteristic preData3Characteristic = peripheral.characteristic("c648e26f-1122-11ea-8d71-362b9e155667");
  if (!preData1Characteristic || !preData2Characteristic || !preData3Characteristic) {
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
        preData3Characteristic.readValue(data3);
        data1Characteristic.writeValue(data1);
        data2Characteristic.writeValue(data2);
        data3Characteristic.writeValue(data3);

        if(analogRead(phonePin)>900){
          data4Characteristic.writeValue(0);
        }else if(analogRead(phonePin)<200){
          data4Characteristic.writeValue(255);
        }
        
        if(analogRead(bookPin)>900){
          data5Characteristic.writeValue(0);
        }else if(analogRead(bookPin)<200){
          data5Characteristic.writeValue(255);
        }
      }
      digitalWrite(LEDpin, LOW);
    }
  }
  
  peripheral.disconnect();
}

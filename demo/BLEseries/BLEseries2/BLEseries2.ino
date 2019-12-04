/*
 * Chair 2
 * 
 * data2Characteristic: 
 * 0 -> not sitted on;
 * 255 -> sitted on;
 */

#include <ArduinoBLE.h>

#define LEDpin 2
#define seatPin A7

int preState = 560;
int tempState = 560;

byte data1 = 0;
byte data2 = 255;

BLEService sencondService("c648e0dc-1122-11ea-8d71-362b9e155667");
BLEByteCharacteristic data1Characteristic("c648e0dd-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);
BLEByteCharacteristic data2Characteristic("c648e0de-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin (9600);
  pinMode(LEDpin, OUTPUT);
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
  if (peripheral){
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

  BLECharacteristic preData1Characteristic = peripheral.characteristic("c648dc41-1122-11ea-8d71-362b9e155667");
  if (!preData1Characteristic) {
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
        data1Characteristic.writeValue(data1);
        
        tempState = analogRead(seatPin);
        if((tempState-preState)>100){
          preState = tempState;
          data2Characteristic.writeValue(255);
          digitalWrite(LED_BUILTIN, HIGH);
        }
        if((tempState-preState)<-100){
          preState = tempState;
          data2Characteristic.writeValue(0);
          digitalWrite(LED_BUILTIN, LOW);
        }
      }
      digitalWrite(LEDpin, LOW);
    }
  }
  
  peripheral.disconnect();
}

/* 
 * This device just advertises sensor data.
 */

#include <ArduinoBLE.h>

#define LEDpin 2

int preState = 560;
int tempState = 560;

byte data1 = 100;

BLEService firstService("c648dc40-1122-11ea-8d71-362b9e155667");
BLEByteCharacteristic data1Characteristic("c648dc41-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin (9600);
  pinMode(LEDpin, OUTPUT);
  pinMode(3, OUTPUT);
  
  BLE.begin();
  BLE.setLocalName("BLEseries1");
  BLE.setAdvertisedService(firstService);
  firstService.addCharacteristic(data1Characteristic);
  BLE.addService(firstService);
  data1Characteristic.writeValue(data1);
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
      digitalWrite(LEDpin, HIGH);
      tempState = analogRead(A0);
      if((tempState-preState)>100){
        preState = tempState;
        data1Characteristic.writeValue(100);
        digitalWrite(3, HIGH);
      }
      if((tempState-preState)<-100){
        preState = tempState;
        data1Characteristic.writeValue(0);
        digitalWrite(3, LOW);
      }
    }
    digitalWrite(LEDpin, LOW);
  }
}

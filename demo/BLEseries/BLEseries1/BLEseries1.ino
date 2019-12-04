/* 
 * Chair 1
 * 
 * data1Characteristic: 
 * 0 -> not sitted on;
 * 255 -> sitted on;
 */

#include <ArduinoBLE.h>

#define LEDpin 2
#define seatPin A0

int preState = 560;
int tempState = 560;

byte data1 = 255;

BLEService firstService("c648dc40-1122-11ea-8d71-362b9e155667");
BLEByteCharacteristic data1Characteristic("c648dc41-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite | BLENotify);

void setup() {
  Serial.begin (9600);
  pinMode(LEDpin, OUTPUT);
  pinMode(seatPin, OUTPUT);
  
  BLE.begin();
  BLE.setLocalName("BLE_chair1_<1>");
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
      
      tempState = analogRead(seatPin);
      if((tempState-preState)>100){
        preState = tempState;
        data1Characteristic.writeValue(255);
      }
      if((tempState-preState)<-100){
        preState = tempState;
        data1Characteristic.writeValue(0);
      }
    }
    digitalWrite(LEDpin, LOW);
  }
}

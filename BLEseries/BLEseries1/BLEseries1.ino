/* 
 * Chair 1
 * 
 * data1Characteristic: 
 * 0 -> not sitted on;
 * 255 -> sitted on;
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

BLEService firstService("c648dc40-1122-11ea-8d71-362b9e155667");
BLEByteCharacteristic data1Characteristic("c648dc41-1122-11ea-8d71-362b9e155667", BLERead | BLEWrite);

void setup() {
  Serial.begin (9600);
  pinMode(LED_BUILTIN, OUTPUT);
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
      digitalWrite(LED_BUILTIN, HIGH);
      
      total -= readings[readIndex];
      readings[readIndex] = analogRead(A7);
      total += readings[readIndex];
      readIndex++;
      if(readIndex>=howMany){
        readIndex = 0;
      }
      sensorValue = total/howMany;
      if(sensorValue<1000){
        if(data1Characteristic.value()==0)
        data1Characteristic.writeValue(255);
      }
      else{
        if(data1Characteristic.value()==255)
        data1Characteristic.writeValue(0);
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
  }
}

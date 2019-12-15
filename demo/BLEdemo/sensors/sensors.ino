#include <ArduinoBLE.h>

//---
//BLE
//---
BLEService sensorService("ede2f3d6-856d-41a8-811b-ed1385c2d44a");
BLEByteCharacteristic chairCharacteristic("ede2f3d7-856d-41a8-811b-ed1385c2d44a", BLERead | BLEWrite | BLEIndicate);

void setup() {
  Serial.begin (9600);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, INPUT);

  //---------
  //BLE setup
  //---------
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("SensorCenter");
  BLE.setAdvertisedService(sensorService);
  sensorService.addCharacteristic(chairCharacteristic);
  BLE.addService(sensorService);
  chairCharacteristic.writeValue(0);
  BLE.advertise();
  
  Serial.println("Sensor Center ready!");
}

void loop() {
  //---------
  //update data
  //---------
   BLEDevice central = BLE.central();
   if (central) {
    digitalWrite(LED_BUILTIN, HIGH);
    while (central.connected()) {
      if(analogRead(A7) < 128 ){
        if (chairCharacteristic.value() != 255)
        chairCharacteristic.writeValue(255);
      }
      else{
        if (chairCharacteristic.value() != 0)
        chairCharacteristic.writeValue(0);
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
  }

  
}

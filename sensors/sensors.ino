#include <ArduinoBLE.h>

#define trigPin 3
#define echoPin 2
#define servoPin 9
#define distanceThreshold 50

//---
//BLE
//---
BLEService sensorService("ede2f3d6-856d-41a8-811b-ed1385c2d44a");
BLEByteCharacteristic chairCharacteristic("ede2f3d7-856d-41a8-811b-ed1385c2d44a", BLERead | BLEWrite);

void setup() {
  Serial.begin (9600);

  //------------
  //sensor setup
  //------------
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

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
  chairCharacteristic.writeValue((byte)0x01);
  BLE.advertise();
  
  Serial.println("Sensor Center ready!");
}

void loop() {
  //-----------------
  // get the distance
  //-----------------
  long duration, distance;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  
  Serial.print(distance);
  Serial.println(" cm");
  delay(500);

  //---------
  //update data
  //---------
   BLEDevice central = BLE.central();
   if (central) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(central.address());
    while (central.connected()) {
      if(distance < distanceThreshold){
        chairCharacteristic.writeValue((byte)0x01);
      }
      else{
        chairCharacteristic.writeValue((byte)0x00);
      }
    }
    digitalWrite(LED_BUILTIN, LOW);
  }

  
}

#include <ArduinoBLE.h>

// servo
#include <Servo.h>
Servo myservo;
const int servoPin = 9;

void setup() {
  Serial.begin(9600);

  //-----------
  //servo setup
  //-----------
  myservo.attach(servoPin);
  myservo.write(0);
  //---------
  //BLE setup
  //---------
  BLE.begin();
  Serial.println("BLE Central");
  // start scanning for peripherals
  BLE.scanForUuid("ede2f3d6-856d-41a8-811b-ed1385c2d44a");
}

void loop() {
  BLEDevice peripheral = BLE.available();
  if (peripheral) {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    if (peripheral.localName() != "SensorCenter") {
      return;
    }
    BLE.stopScan();
    getData(peripheral);
    BLE.scanForUuid("ede2f3d6-856d-41a8-811b-ed1385c2d44a");
  }
}

void getData(BLEDevice peripheral) {
  if (peripheral.connect()) {
  } else {
    return;
  }

  if (peripheral.discoverAttributes()) {
  } else {
    peripheral.disconnect();
    return;
  }

  BLECharacteristic chairCharacteristic = peripheral.characteristic("ede2f3d7-856d-41a8-811b-ed1385c2d44a");
  if (!chairCharacteristic) {
    peripheral.disconnect();
    return;
  }
  byte value = 0;
  chairCharacteristic.readValue(value);
  if(value==1){
    myservo.write(90);
    delay(1000);
    myservo.write(0);
  }
  peripheral.disconnect();
}

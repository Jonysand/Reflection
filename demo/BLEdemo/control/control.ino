#include <ArduinoBLE.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define LED_COUNT 84

int nBytesRead;
byte data;
byte dataReturned;
uint32_t background;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);

  //----------
  // LED setup
  //----------
  strip.begin();
  background = strip.Color(181, 200, 230);
  strip.fill(background, 0, 84);
  strip.show();

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
  chairCharacteristic.setEventHandler(BLEWritten, changeBG);
  while (peripheral.connected()) {
    if (chairCharacteristic.valueUpdated()) {
      chairCharacteristic.readValue(dataReturned);
      Serial.println("updated");
      data = dataReturned;
    }
    Serial.println(data);
    //    Serial.println(*bytesReadString, DEC);
    
  }

  peripheral.disconnect();
}

int cord2index(int x, int y) {
  if (x % 2 == 0) {
    return 12 * x + y;
  } else {
    return 12 * (x + 1) - 1 - y;
  }
}

void changeBG(BLEDevice central, BLECharacteristic characteristic){
  data = *characteristic.value();
  if (*characteristic.value() > 128) {
      for (int y = 0; y < 12; y++) {
        for (int x = 0; x < 7; x++) {
          int color1 = 1820 * y + millis() * 10;
          strip.setPixelColor(cord2index(x, y), strip.gamma32(strip.ColorHSV(color1, 255, 255)));
        }
      }
      strip.show();
    } else {
      strip.fill(background, 0, 84);
      strip.show();
    }
}

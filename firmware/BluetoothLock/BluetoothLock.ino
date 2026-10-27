#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Stepper.h>
//test
const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, 27, 25, 26, 33);

#define SERVICE_UUID        "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

BLECharacteristic *pCharacteristic;

const int RED_PIN = 18;
const int GREEN_PIN = 19;

void setColor(bool red, bool green) {
  if (red && green) {
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 190);
  } else {
    analogWrite(RED_PIN, red ? 255 : 0);
    analogWrite(GREEN_PIN, green ? 255 : 0);
  }
}

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    String value = pCharacteristic->getValue();
    Serial.println(value.c_str());

    if (value == "123") {
      setColor(true, true);    // Yellow: moving
      myStepper.step(512);
      setColor(false, true);   // Green: unlock movement finished
      Serial.println("Unlocked!");
}

    if (value == "000") {
      setColor(true, true);    // Yellow: moving
      myStepper.step(-512);
      setColor(true, false);   // Red: lock movement finished
      Serial.println("Locked!");
}
  }
};

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  setColor(false, false); // Off: position unknown at startup
  myStepper.setSpeed(10);

  BLEDevice::init("ESP32Lock");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  BLEDevice::getAdvertising()->start();
  Serial.println("Waiting for BLE connection...");
}

void loop() {}

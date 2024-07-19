#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;

int rssiThreshold = -70; // Adjust this threshold as needed

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Device connected");
    }

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Device disconnected");
    }
};

void setup() {
  Serial.begin(115200);
  
  BLEDevice::init("ESP32_BLE");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  
  pService->start();
  pServer->getAdvertising()->addServiceUUID(pService->getUUID());
  pServer->getAdvertising()->start();
  
  BLEDevice::setMTU(500);

  pServer->setCallbacks(new MyServerCallbacks());
}

void loop() {
  BLEDevice::startAdvertising();
  delay(100);

  BLEDevice::stopAdvertising();
  delay(1000);

  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  int rssi = pAdvertising->getRSSI();

  Serial.print("RSSI: ");
  Serial.println(rssi);

  if (rssi > rssiThreshold) {
    Serial.println("Phone is close to ESP32");
    // Add actions to perform when phone is close
  } else {
    Serial.println("Phone is far from ESP32");
    // Add actions to perform when phone is far
  }

  delay(1000);
}

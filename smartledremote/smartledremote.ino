#include <ArduinoBLE.h>

#define ON_BUTTON 2
#define OFF_BUTTON 3
#define AUTO_BUTTON 4
#define UP_BUTTON 5
#define DOWN_BUTTON 6

int brightness = 0;

BLEService remoteService("180A"); // 서비스 UUID
BLECharacteristic remoteCharacteristic("2A57", BLEWrite, 10); // 특성 UUID

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // 버튼 핀 초기화
  pinMode(ON_BUTTON, INPUT_PULLUP);
  pinMode(OFF_BUTTON, INPUT_PULLUP);
  pinMode(AUTO_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);

  // BLE 초기화
  if (!BLE.begin()) {
    Serial.println("BLE 초기화 실패!");
    while (1);
  }

  Serial.println("BLE 초기화 성공!");

  // BLE Central 시작
  BLE.scanForUuid("180A");
  Serial.println("BLE 스캔 시작...");
}

void loop() {
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    // Peripheral 디바이스 발견
    Serial.print("발견된 디바이스: ");
    Serial.println(peripheral.localName());

    if (peripheral.localName() == "SmartLED") { // 대상 장치 이름 확인
      Serial.println("SmartLED 연결 시도...");
      BLE.stopScan();

      if (peripheral.connect()) {
        Serial.println("SmartLED 연결 성공!");
        if (peripheral.discoverService("180A")) {
          BLECharacteristic characteristic = peripheral.characteristic("2A57");
          if (characteristic) {
            controlLED(peripheral, characteristic);
          } else {
            Serial.println("Characteristic 찾기 실패!");
          }
        } else {
          Serial.println("Service 찾기 실패!");
        }
      } else {
        Serial.println("SmartLED 연결 실패.");
      }

      BLE.scanForUuid("180A"); // 재스캔 시작
    }
  }
}

void controlLED(BLEDevice &peripheral, BLECharacteristic &characteristic) {
  while (peripheral.connected()) {
    if (digitalRead(ON_BUTTON) == LOW) {
      sendCommand(characteristic, "1");
      delay(200);
    }
    if (digitalRead(OFF_BUTTON) == LOW) {
      sendCommand(characteristic, "0");
      delay(200);
    }
    if (digitalRead(AUTO_BUTTON) == LOW) {
      sendCommand(characteristic, "2");
      delay(200);
    }
    if (digitalRead(UP_BUTTON) == LOW) {
      if (brightness < 9) brightness++;
      sendCommand(characteristic, "B" + String(brightness));
      delay(200);
    }
    if (digitalRead(DOWN_BUTTON) == LOW) {
      if (brightness > 0) brightness--;
      sendCommand(characteristic, "B" + String(brightness));
      delay(200);
    }
  }

  Serial.println("연결이 끊어졌습니다.");
}

void sendCommand(BLECharacteristic &characteristic, String command) {
  characteristic.writeValue(command.c_str());
  Serial.print("명령 전송: ");
  Serial.println(command);
}

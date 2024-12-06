#include <ArduinoBLE.h>

#define ON_BUTTON 2
#define OFF_BUTTON 3
#define AUTO_BUTTON 4
#define UP_BUTTON 5
#define DOWN_BUTTON 6

int brightness = 0;

BLEService remoteService("180A"); // 서비스 UUID
BLECharacteristic remoteCharacteristic("2A57", BLEWrite, 10); // 특성 UUID

bool prevOnState = HIGH;
bool prevOffState = HIGH;
bool prevAutoState = HIGH;
bool prevUpState = HIGH;
bool prevDownState = HIGH;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(ON_BUTTON, INPUT_PULLUP);
  pinMode(OFF_BUTTON, INPUT_PULLUP);
  pinMode(AUTO_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);

  if (!BLE.begin()) {
    Serial.println("BLE 초기화 실패!");
    while (1);
  }

  Serial.println("BLE 초기화 성공!");
  BLE.scanForUuid("180A");
  Serial.println("BLE 스캔 시작...");
}

void loop() {
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    Serial.print("발견된 디바이스: ");
    Serial.println(peripheral.localName());

    if (peripheral.localName() == "SmartLED") {
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

      BLE.scanForUuid("180A");
    }
  }
}

void controlLED(BLEDevice &peripheral, BLECharacteristic &characteristic) {
  while (peripheral.connected()) {
    // 버튼의 현재 상태 읽기
    bool currentOnState = digitalRead(ON_BUTTON);
    bool currentOffState = digitalRead(OFF_BUTTON);
    bool currentAutoState = digitalRead(AUTO_BUTTON);
    bool currentUpState = digitalRead(UP_BUTTON);
    bool currentDownState = digitalRead(DOWN_BUTTON);

    // ON 버튼 상태 변화 감지
    if (currentOnState == LOW && prevOnState == HIGH) {
      sendCommand(characteristic, "1");
    }

    // OFF 버튼 상태 변화 감지
    if (currentOffState == LOW && prevOffState == HIGH) {
      sendCommand(characteristic, "0");
    }

    // AUTO 버튼 상태 변화 감지
    if (currentAutoState == LOW && prevAutoState == HIGH) {
      sendCommand(characteristic, "2");
    }

    // UP 버튼 상태 변화 감지
    if (currentUpState == LOW && prevUpState == HIGH) {
      if (brightness < 9) brightness++;
      sendCommand(characteristic, "B" + String(brightness));
    }

    // DOWN 버튼 상태 변화 감지
    if (currentDownState == LOW && prevDownState == HIGH) {
      if (brightness > 0) brightness--;
      sendCommand(characteristic, "B" + String(brightness));
    }

    // 이전 상태 갱신
    prevOnState = currentOnState;
    prevOffState = currentOffState;
    prevAutoState = currentAutoState;
    prevUpState = currentUpState;
    prevDownState = currentDownState;
  }

  Serial.println("연결이 끊어졌습니다.");
}

void sendCommand(BLECharacteristic &characteristic, String command) {
  characteristic.writeValue(command.c_str());
  Serial.print("명령 전송: ");
  Serial.println(command);
}

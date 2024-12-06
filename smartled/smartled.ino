#include <ArduinoBLE.h>

#define LED_PIN 2      // LED 핀 (PWM)
#define LIGHT_SENSOR A2 // 조도 센서 핀

int brightness = 0;       // 밝기 (0~255)
bool autoMode = false;    // AUTO 모드 여부
bool ledOn = false;       // LED ON/OFF 상태

BLEService ledService("180A"); // LED 서비스
BLEStringCharacteristic ledCharacteristic("2A57", BLERead | BLEWrite, 10); // LED 특성

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // BLE 초기화
  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("SmartLED");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(ledCharacteristic);
  BLE.addService(ledService);

  ledCharacteristic.writeValue("0");

  BLE.advertise();
  Serial.println("BLE device is now advertising");

  // LED 핀 및 센서 초기화
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);
}

void loop() {
  // BLE 클라이언트 연결 관리
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      if (ledCharacteristic.written()) {
        String value = ledCharacteristic.value();
        handleCommand(value);
      }

      if (autoMode) {
        int lightValue = analogRead(LIGHT_SENSOR); // 조도 센서 값 읽기
        if (lightValue < 500) { // 어두우면 LED 켜기
          Serial.println(lightValue);
          delay(100);
          analogWrite(LED_PIN, brightness);
          ledOn = true;
        } else { // 밝으면 LED 끄기
          Serial.println(lightValue);
          delay(100);
          analogWrite(LED_PIN, 0);
          ledOn = false;
        }
      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

void handleCommand(String value) {
  if (value == "0") {
    analogWrite(LED_PIN, 0);
    ledOn = false;
    autoMode = false;
    Serial.println("LED OFF");
  } else if (value == "1") {
    analogWrite(LED_PIN, brightness);
    ledOn = true;
    autoMode = false;
    Serial.println("LED ON");
  } else if (value == "2") {
    autoMode = true;
    Serial.println("AUTO MODE ON");
  } else if (value.startsWith("B")) { // 밝기 조정 (B0 ~ B9)
    int level = value.substring(1).toInt();
    if (level >= 0 && level <= 9) {
      brightness = map(level, 0, 9, 0, 255); // 0~9를 0~255로 매핑
      if (ledOn) {
        analogWrite(LED_PIN, brightness);
      }
      Serial.print("Brightness set to: ");
      Serial.println(brightness);
    }
  }
}

# Smart LED and Remote

💡 **A Smart LED System with Remote Control** for IoT Project

---

## 📸 Preview

<div align="center">
  <img src="images/smart_led.png" alt="Smart LED" width="200">
  <img src="images/smart_remote.png" alt="Remote Control" width="200">
</div>

---

## 🛠️ Features

- **Smart LED Control**:  
  - ON/OFF and brightness adjustment with BLE commands.  
  - Automatic mode based on ambient light.  

- **Remote Control**:  
  - Buttons for ON, OFF, AUTO, Brightness Up, and Brightness Down.  
  - BLE communication with the Smart LED.  

- **BLE Integration**:  
  - `Service UUID`: `180A`  
  - `Characteristic UUID`: `2A57`

---

## 🚀 Implementation Steps

### **1. Smart LED (`smartled.ino`)**
- **Peripheral Device**:  
  - Contains an LED and light sensor.  
  - Responds to BLE commands to control LED state and brightness.  
- **Hardware Requirements**:  
  - LED (connected to pin `2`)  
  - Light sensor (connected to pin `A2`)  

### **2. Remote Control (`smartledremote.ino`)**
- **Central Device**:  
  - Sends commands to the Smart LED via BLE.  
- **Hardware Requirements**:  
  - 5 buttons for control:  
    - ON (`2`), OFF (`3`), AUTO (`4`), UP (`5`), DOWN (`6`)

---

## ⚙️ BLE Command Mapping

| **Button**       | **Command**  | **Action**                 |
|-------------------|--------------|----------------------------|
| OFF              | `0`          | Turn off LED              |
| ON               | `1`          | Turn on LED               |
| AUTO             | `2`          | Enable automatic mode     |
| Brightness UP    | `10 + (current brightness + 1)` | Increase brightness |
| Brightness DOWN  | `10 + (current brightness - 1)` | Decrease brightness |

---

## 💻 Example Commands

1. **Turn LED ON**  
   - Remote sends `1` to the LED via BLE.  
   - LED turns on.

2. **Enable AUTO Mode**  
   - Remote sends `2`.  
   - LED toggles based on light sensor readings.

3. **Adjust Brightness**  
   - Current brightness is `5`.  
   - Remote sends `16` (`5 + 1 + 10`) for UP, or `14` (`5 - 1 + 10`) for DOWN.  

---

## 📂 Submission

- `smartled.ino`  
- `smartledremote.ino`  

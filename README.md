# ESP32 Smart Home Automation & Embedded Networking

[![Firmware CI](https://github.com/VPariz/esp32-home-automation-iot/actions/workflows/ci.yml/badge.svg)](https://github.com/VPariz/esp32-home-automation-iot/actions/workflows/ci.yml)
[![MCU](https://img.shields.io/badge/MCU-ESP32%20DevKit%20V1-E7352C?style=flat-square&logo=espressif&logoColor=white)](https://espressif.com/)
[![Framework](https://img.shields.io/badge/Framework-Arduino%20Core%20%2F%20ESP--IDF-00979D?style=flat-square&logo=arduino&logoColor=white)](https://github.com/espressif/arduino-esp32)
[![Protocol](https://img.shields.io/badge/Protocols-HTTP%20%7C%20Wi--Fi%20%7C%20Bluetooth%20SPP-00599C?style=flat-square)](https://en.wikipedia.org/wiki/Internet_protocol_suite)
[![Institution](https://img.shields.io/badge/IME-Electronics_Engineering-104911?style=flat-square)](https://ime.eb.mil.br/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](LICENSE)

Embedded IoT home automation platform and practical laboratory guide developed on the **ESP32 (Tensilica Xtensa Dual-Core 32-bit LX6)** microcontroller. Features an embedded HTTP webserver with responsive dark-mode dashboard, Wi-Fi failover (Station + SoftAP contingency), Bluetooth Classic Serial Port Profile (SPP) telemetry, and optocoupled 4-channel AC mains relay actuation.

Includes a comprehensive 9-page engineering guide in LaTeX and compiled PDF covering electrical schematics, logic-level matching, strapping pin isolation, and active-LOW boot safety.

---

## 📂 Repository Structure

```
.
├── src/
│   ├── 01_teste_reles/
│   │   └── 01_teste_reles.ino              # Sequential relay cycle & boot-state safety verification
│   ├── 02_webserver_4_lampadas/
│   │   └── 02_webserver_4_lampadas.ino     # Embedded HTTP Webserver (port 80) with STA + SoftAP fallback
│   └── 03_bluetooth_controle/
│       └── 03_bluetooth_controle.ino       # Wireless bidirectional serial control via Bluetooth SPP
├── docs/
│   ├── TUTORIAL_GERAL_ESP32.pdf            # Compiled 9-page technical bench guide & hardware design
│   └── TUTORIAL_GERAL_ESP32.tex            # LaTeX source document with electrical schematics & pinout
├── .gitattributes                          # GitHub Linguist documentation overrides
├── .gitignore
├── LICENSE                                 # MIT License
└── README.md
```

---

## ⚡ Technical Architecture

### 1. Embedded HTTP Webserver (`02_webserver_4_lampadas.ino`)
* **Dual-Mode Networking:** Attempts connection to a local Wi-Fi router (Station / STA mode) with dynamic DHCP acquisition; automatically falls back to an autonomous local Access Point (SoftAP: `ESP32-Automacao`, `192.168.4.1`) upon connection timeout.
* **Lightweight UI Delivery:** Responsive HTML5/CSS3 dark-themed control interface served directly from MCU flash memory with zero external CDN dependencies.
* **REST-like State Toggling:** Real-time state query and pin actuation via parameterized HTTP GET requests (`/?rele=1`, `/?rele=all_on`, etc.).

### 2. Bluetooth Serial Telemetry (`03_bluetooth_controle.ino`)
* **Wireless Serial Bridge:** Implements Bluetooth Classic SPP (*Serial Port Profile*) using `BluetoothSerial.h`.
* **ASCII Command Parser:** Telemetry and remote actuation via serial terminal commands:
  * `L1 ON` / `L1 OFF` ... `L4 ON` / `L4 OFF` (individual load toggling)
  * `ALL ON` / `ALL OFF` (collective emergency/master switches)
  * `STATUS` (queries relay digital states and uptime)
  * `HELP` (returns valid command list)

### 3. Hardware & Power Safety (`01_teste_reles.ino`)
* **Active-LOW Mitigation:** Relays are active-LOW optocoupled modules; GPIOs are forced `HIGH` *prior* to `pinMode(OUTPUT)` to prevent spurious relay clicks during ESP32 boot.
* **Strapping Pin Isolation:** Avoids boot-sensitive GPIOs (0, 2, 12, 15) to guarantee clean flashing and stable hardware initialization.
* **Control Pin Assignment:**
  * Relay 1 (`IN1`): **GPIO 25** (Room 1 / Living Room)
  * Relay 2 (`IN2`): **GPIO 26** (Room 2 / Bedroom)
  * Relay 3 (`IN3`): **GPIO 32** (Room 3 / Kitchen)
  * Relay 4 (`IN4`): **GPIO 33** (Room 4 / Balcony)

---

## 📑 Technical Documentation (IME)

The `docs/` folder contains the full technical report and laboratory manual:
* 📄 [**`docs/TUTORIAL_GERAL_ESP32.pdf`**](docs/TUTORIAL_GERAL_ESP32.pdf): Complete 9-page laboratory guide covering:
  1. ESP32 DevKit V1 architecture and CP2102/CH340 USB-UART interfacing.
  2. Electrical power distribution and optocoupler isolation for AC mains.
  3. Active-LOW logic vs. Active-HIGH safety comparisons.
  4. Brownout detector management and capacitor filtering against inductive switching spikes.
  5. Step-by-step firmware deployment and troubleshooting workflow.

---

## 🚀 Getting Started

### Prerequisites
* [Arduino IDE 2.x](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
* **ESP32 Arduino Core**: In Arduino IDE, add `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json` to Additional Board Manager URLs and install `esp32` by Espressif.

### Flashing Firmware
1. Connect the ESP32 DevKit V1 to your computer via micro-USB.
2. Select Board: **DOIT ESP32 DEVKIT V1** (or ESP32 Dev Module).
3. Select Port: The appropriate COM port (e.g., `COM3` on Windows).
4. Configure upload speed to `921600` or `115200`.
5. Open any sketch in `src/` (`01_teste_reles`, `02_webserver_4_lampadas`, or `03_bluetooth_controle`).
6. Click **Upload**.

---

## 📜 Author & License

* **Author:** João Victor Pariz ([@VPariz](https://github.com/VPariz))
* **Institution:** Instituto Militar de Engenharia (IME) — Electronics Engineering
* **License:** This repository is open-sourced under the [MIT License](LICENSE).

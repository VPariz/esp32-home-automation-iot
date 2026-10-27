# Network Programming in C & ESP32 IoT Automation

[![Language](https://img.shields.io/badge/Language-C-00599C?style=flat-square&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Microcontroller](https://img.shields.io/badge/MCU-ESP32-E7352C?style=flat-square&logo=espressif&logoColor=white)](https://espressif.com/)
[![Networking](https://img.shields.io/badge/Sockets-POSIX%20%7C%20Winsock2-2D3748?style=flat-square)](https://en.wikipedia.org/wiki/Berkeley_sockets)
[![Institution](https://img.shields.io/badge/IME-Computer_Networks_I-104911?style=flat-square)](https://ime.eb.mil.br/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](LICENSE)

Practical low-level network programming with socket APIs in C (UDP multi-user chat and TCP chunked file transfer using BSD Sockets and Winsock2) alongside embedded home automation firmware on ESP32 microcontrollers. Developed for the **Computer Networks I** laboratory at the **Military Institute of Engineering (IME)**.

---

## 📂 Repository Structure

```
lab-redes-c-esp32/
├── c_sockets/
│   ├── chat_udp/
│   │   ├── servidor-chat-2-usuarios-udp.c # UDP relay server between two endpoints
│   │   └── cliente-chat-2-usuarios-udp.c  # Asynchronous message exchange client
│   └── transferencia_arquivos_tcp/
│       ├── servidor-transf-arquivo-tcp.c # Binary file receiver with byte counting
│       └── cliente-transf-arquivo-tcp.c  # Chunked binary file reader and transmitter
└── esp32_iot/
    ├── src/
    │   ├── 01_teste_reles/                  # Sequential solid-state relay actuation
    │   ├── 02_webserver_4_lampadas/         # Embedded HTTP server with responsive web dashboard
    │   └── 03_bluetooth_controle/           # Bluetooth Classic SPP telemetry & remote control
    ├── TUTORIAL_GERAL_ESP32.tex             # LaTeX hardware design and architecture report
    └── TUTORIAL_GERAL_ESP32.pdf             # Compiled 9-page technical engineering report
```

---

## ⚡ Technical Highlights

* **C Sockets System Programming:**
  * Direct POSIX / Winsock system calls: `socket()`, `bind()`, `listen()`, `accept()`, `send()`, `recv()`, `sendto()`, and `recvfrom()`.
  * **UDP Chat:** Low-overhead datagram transmission for low-latency messaging between two endpoints on local subnets with `select()` multiplexing.
  * **TCP File Transfer:** Reliable, connection-oriented byte streams for chunked binary file transfer with checksum and byte counter validation.
* **ESP32 Embedded Firmware:**
  * Lightweight embedded HTTP server serving HTML/CSS directly from flash memory.
  * Resilient Wi-Fi failover: attempts local station connection (STA mode); falls back to standalone SoftAP access point upon connection timeout.
  * Optocoupled power relay switching with inductive flyback protection for AC mains loads.
  * Wireless bidirectional serial communication via Bluetooth Classic SPP.

---

## 🚀 Building & Running C Sockets

### On Linux (GCC)

```bash
# UDP Chat
gcc c_sockets/chat_udp/servidor-chat-2-usuarios-udp.c -o servidor_chat
gcc c_sockets/chat_udp/cliente-chat-2-usuarios-udp.c -o cliente_chat

# In separate terminals:
./servidor_chat
./cliente_chat

# TCP File Transfer
gcc c_sockets/transferencia_arquivos_tcp/servidor-transf-arquivo-tcp.c -o servidor_transf
gcc c_sockets/transferencia_arquivos_tcp/cliente-transf-arquivo-tcp.c -o cliente_transf
```

### On Windows (MinGW / GCC)

Link against `ws2_32` for Windows Sockets:

```bash
# UDP Chat
gcc c_sockets/chat_udp/servidor-chat-2-usuarios-udp.c -o servidor_chat.exe -lws2_32
gcc c_sockets/chat_udp/cliente-chat-2-usuarios-udp.c -o cliente_chat.exe -lws2_32

# TCP File Transfer
gcc c_sockets/transferencia_arquivos_tcp/servidor-transf-arquivo-tcp.c -o servidor_transf.exe -lws2_32
gcc c_sockets/transferencia_arquivos_tcp/cliente-transf-arquivo-tcp.c -o cliente_transf.exe -lws2_32
```

---

## ⚡ Flashing the ESP32

1. Open the [Arduino IDE](https://www.arduino.cc/en/software) or PlatformIO.
2. In **Boards Manager**, install the **esp32** package by Espressif Systems.
3. Select board: **DOIT ESP32 DEVKIT V1**.
4. Open the target `.ino` sketch under `esp32_iot/src/` and click **Upload**.

---

## 📜 License
This project is licensed under the **MIT License** — see the `LICENSE` file for details.

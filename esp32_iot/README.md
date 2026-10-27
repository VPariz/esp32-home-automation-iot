# Laboratório de Redes & IoT: Automação Residencial com ESP32

**Instituto Militar de Engenharia (IME) — Departamento de Engenharia Elétrica e Eletrônica**  
**Área:** Redes de Computadores, Sistemas Embarcados & Internet das Coisas (IoT)

---

## 📌 Visão Geral do Projeto

Este diretório contém a documentação técnica, esquemáticos de hardware e firmwares desenvolvidos para experimentos práticos de **Internet das Coisas (IoT)**, **Bluetooth Classic (SPP)** e **Servidores Web Embarcados (HTTP/TCP)** utilizando o microcontrolador **ESP32 DevKit V1 (30 pinos)** e um **módulo de 4 relés optoacoplados** para acionamento de cargas em corrente alternada.

---

## 📑 Guia Técnico Completo (LaTeX)

* 📝 **[TUTORIAL_GERAL_ESP32.tex](./TUTORIAL_GERAL_ESP32.tex)**: Código-fonte em LaTeX do guia completo de bancada, cobrindo arquitetura elétrica, proteção de pinos e protocolos de comunicação.

### Tópicos Abordados no Guia:
1. **Integração de Redes e Sistemas Ciberfísicos:** Transição da camada de transporte via Sockets para controle físico de atuadores em sistemas embarcados.
2. **Configuração da Toolchain:** Preparação da Arduino IDE 2.x, instalação do Board Core oficial da Espressif (`esptool.py`) e drivers USB-Serial CP210x / CH340.
3. **Mapeamento de Hardware e Pinagem Segura:** Compatibilidade de níveis lógicos (3.3V vs 5V), isolamento de pinos de *strapping* (GPIOs 0, 2, 12, 15) e tratamento de **Lógica Active-LOW** (evitando atracamento espúrio no boot).
4. **Esquemático de Ligação de Potência:** Mapeamento dos sinais de controle (`IN1` a `IN4` nos GPIOs `25, 26, 32, 33`) e chaveamento de fase nos contatos `COM` e `NA`.
5. **Experimento 1:** Teste sequencial e coletivo de acionamento eletromecânico em bancada.
6. **Experimento 2:** Servidor Web HTTP Wi-Fi em modo dual (*Station STA* + contingência automática em *SoftAP*) com interface responsiva embarcada.
7. **Experimento 3:** Controle por comandos ASCII e telemetria via Bluetooth Serial (`BluetoothSerial.h`).
8. **Guia de Troubleshooting:** Diagnóstico de falhas de sincronismo no upload, *Brownout Detector* por queda de tensão e proteção de inicialização.

---

## 💻 Firmwares Disponíveis (`src/`)

* 📂 **[01_teste_reles.ino](./src/01_teste_reles/01_teste_reles.ino)**:
  * Inicialização segura dos relés em nível lógico alto (`HIGH` = desligado em módulos Active-LOW).
  * Ciclo sequencial individual e acionamento simultâneo com telemetria via Monitor Serial (`115200 bps`).
* 📂 **[02_webserver_4_lampadas.ino](./src/02_webserver_4_lampadas/02_webserver_4_lampadas.ino)**:
  * Servidor HTTP nativo na porta TCP `80`.
  * Conexão Wi-Fi Station com fallback automático para ponto de acesso próprio (`ESP32-Automacao`, IP `192.168.4.1`).
  * Interface web responsiva em Dark UI servida diretamente da memória flash.
* 📂 **[03_bluetooth_controle.ino](./src/03_bluetooth_controle/03_bluetooth_controle.ino)**:
  * Comunicação sem fio via perfil SPP (*Serial Port Profile*).
  * Parser de comandos textuais: `L1 ON/OFF`, `L2 ON/OFF`, `L3 ON/OFF`, `L4 ON/OFF`, `ALL ON/OFF`, `STATUS` e `HELP`.

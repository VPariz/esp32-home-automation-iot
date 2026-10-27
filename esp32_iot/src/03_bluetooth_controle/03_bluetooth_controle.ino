/*
 * ============================================================================
 * Projeto: Laboratório de Redes I - Automação Residencial com ESP32
 * Roteiro 3: Controle e Telemetria Sem Fio via Bluetooth Classic (SPP)
 * Plataforma: ESP32 DevKit V1 (30 pinos) + Módulo Relés Optoacoplados
 * Domínio: Internet das Coisas (IoT) & Sistemas Embarcados
 * ============================================================================
 *
 * Descrição:
 * Permite parear qualquer smartphone ou computador via Bluetooth e controlar
 * individualmente as 4 lâmpadas através de comandos de texto via Serial Bluetooth.
 * Aplicativo recomendado no smartphone: "Serial Bluetooth Terminal" (Android/iOS).
 *
 * Comandos Suportados via Bluetooth:
 *   - L1 ON  / L1 OFF  : Liga/Desliga Lâmpada 1 (Sala)
 *   - L2 ON  / L2 OFF  : Liga/Desliga Lâmpada 2 (Quarto)
 *   - L3 ON  / L3 OFF  : Liga/Desliga Lâmpada 3 (Cozinha)
 *   - L4 ON  / L4 OFF  : Liga/Desliga Lâmpada 4 (Varanda)
 *   - ALL ON / ALL OFF : Liga/Desliga todas as lâmpadas simultaneamente
 *   - STATUS           : Retorna o estado atual de todas as cargas
 *   - HELP             : Exibe menu de comandos
 */

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error "Bluetooth desabilitado! Habilite nas configuracoes do ESP32."
#endif

BluetoothSerial SerialBT;

#define RELAY_1 25
#define RELAY_2 26
#define RELAY_3 32
#define RELAY_4 33

#define RELAY_ON  LOW
#define RELAY_OFF HIGH

const int relayPins[4] = {RELAY_1, RELAY_2, RELAY_3, RELAY_4};
const char* lampNames[4] = {"Sala", "Quarto", "Cozinha", "Varanda"};
bool relayStates[4] = {false, false, false, false};

void setRelay(int idx, bool on) {
    if (idx >= 0 && idx < 4) {
        relayStates[idx] = on;
        digitalWrite(relayPins[idx], on ? RELAY_ON : RELAY_OFF);
        String msg = "[OK] " + String(lampNames[idx]) + " -> " + (on ? "LIGADA" : "DESLIGADA") + "\r\n";
        SerialBT.print(msg);
        Serial.print(msg);
    }
}

void printStatus() {
    SerialBT.println("\r\n--- ESTADO DAS LAMPADAS ---");
    for (int i = 0; i < 4; i++) {
        SerialBT.printf("L%d (%s): %s\r\n", i + 1, lampNames[i], relayStates[i] ? "[ACESO]" : "[APAGADO]");
    }
    SerialBT.println("---------------------------\r\n");
}

void printHelp() {
    SerialBT.println("\r\n=== MENU DE COMANDOS BLUETOOTH ===");
    SerialBT.println("  L1 ON / L1 OFF  -> Lampada 1 (Sala)");
    SerialBT.println("  L2 ON / L2 OFF  -> Lampada 2 (Quarto)");
    SerialBT.println("  L3 ON / L3 OFF  -> Lampada 3 (Cozinha)");
    SerialBT.println("  L4 ON / L4 OFF  -> Lampada 4 (Varanda)");
    SerialBT.println("  ALL ON / ALL OFF-> Todas as Lampadas");
    SerialBT.println("  STATUS          -> Estado atual");
    SerialBT.println("  HELP            -> Este menu");
    SerialBT.println("==================================\r\n");
}

void processCommand(String cmd) {
    cmd.trim();
    cmd.toUpperCase();
    if (cmd.length() == 0) return;

    Serial.println("[BT RECEBIDO] " + cmd);

    if (cmd == "L1 ON") setRelay(0, true);
    else if (cmd == "L1 OFF") setRelay(0, false);
    else if (cmd == "L2 ON") setRelay(1, true);
    else if (cmd == "L2 OFF") setRelay(1, false);
    else if (cmd == "L3 ON") setRelay(2, true);
    else if (cmd == "L3 OFF") setRelay(2, false);
    else if (cmd == "L4 ON") setRelay(3, true);
    else if (cmd == "L4 OFF") setRelay(3, false);
    else if (cmd == "ALL ON") { for(int i=0; i<4; i++) setRelay(i, true); }
    else if (cmd == "ALL OFF") { for(int i=0; i<4; i++) setRelay(i, false); }
    else if (cmd == "STATUS") printStatus();
    else if (cmd == "HELP") printHelp();
    else {
        SerialBT.println("[ERRO] Comando nao reconhecido. Envie HELP para ver as opcoes.");
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Inicialização dos relés
    for (int i = 0; i < 4; i++) {
        digitalWrite(relayPins[i], RELAY_OFF);
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], RELAY_OFF);
    }

    // Inicializa o Bluetooth com nome legível
    const char* btDeviceName = "ESP32-BT-Gateway";
    SerialBT.begin(btDeviceName);
    Serial.printf("[Bluetooth Iniciado] Dispositivo pronto para pareamento: %s\n", btDeviceName);
}

void loop() {
    // Leitura do canal Bluetooth
    if (SerialBT.available()) {
        String input = SerialBT.readStringUntil('\n');
        processCommand(input);
    }

    // Permite comandos também pelo Monitor Serial USB (debug)
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        processCommand(input);
    }
}

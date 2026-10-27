/*
 * ============================================================================
 * Projeto: Laboratório de Redes I - Automação Residencial com ESP32
 * Roteiro 1: Teste de Bancada e Acionamento Sequencial de 4 Relés
 * Plataforma: ESP32 DevKit V1 (30 pinos) + Módulo Relés Optoacoplados
 * Domínio: Internet das Coisas (IoT) & Sistemas Embarcados
 * ============================================================================
 * 
 * Descrição:
 * Este código realiza o teste inicial do módulo de 4 relés do kit Eletrogate.
 * ATENÇÃO: A grande maioria dos módulos de relé para microcontroladores opera
 * em LÓGICA ATIVA EM NÍVEL BAIXO (Active-LOW):
 *   - Nível LOW (0V) -> Relé ATRACADO (Lâmpada Acesa / Contato NA Fechado)
 *   - Nível HIGH (3.3V) -> Relé DESLIGADO (Lâmpada Apagada / Contato NA Aberto)
 *
 * Mapeamento sugerido de GPIOs seguros no ESP32 DevKit V1 (30 pinos):
 *   - IN1 -> GPIO 25 (Lâmpada 1 - Sala)
 *   - IN2 -> GPIO 26 (Lâmpada 2 - Quarto)
 *   - IN3 -> GPIO 32 (Lâmpada 3 - Cozinha)
 *   - IN4 -> GPIO 33 (Lâmpada 4 - Varanda)
 * (Evitar usar GPIOs de strapping como 0, 2, 12 e 15 na inicialização)
 */

#define RELAY_1 25
#define RELAY_2 26
#define RELAY_3 32
#define RELAY_4 33

// Definições de estados para Lógica Active-LOW
#define RELAY_ON  LOW
#define RELAY_OFF HIGH

const int relayPins[4] = {RELAY_1, RELAY_2, RELAY_3, RELAY_4};
const char* lampNames[4] = {"Lâmpada 1 (Sala)", "Lâmpada 2 (Quarto)", "Lâmpada 3 (Cozinha)", "Lâmpada 4 (Varanda)"};

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n==========================================");
    Serial.println("  Embedded IoT Lab - Relay Controller    ");
    Serial.println("  Teste Inicial do Módulo de 4 Relés     ");
    Serial.println("==========================================");

    // Inicialização segura dos pinos (garante que todos iniciem DESLIGADOS)
    for (int i = 0; i < 4; i++) {
        digitalWrite(relayPins[i], RELAY_OFF);
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], RELAY_OFF);
    }
    Serial.println("[OK] Todos os relés inicializados em estado DESLIGADO (HIGH).");
}

void loop() {
    Serial.println("\n--- Iniciando ciclo de acionamento sequencial ---");

    // Aciona um a um
    for (int i = 0; i < 4; i++) {
        Serial.printf("[ACIONANDO] %s no GPIO %d...\n", lampNames[i], relayPins[i]);
        digitalWrite(relayPins[i], RELAY_ON);
        delay(1500);
        
        Serial.printf("[DESLIGANDO] %s...\n", lampNames[i]);
        digitalWrite(relayPins[i], RELAY_OFF);
        delay(500);
    }

    // Aciona todos juntos por 2 segundos
    Serial.println("[TESTE COLETIVO] Acionando todas as 4 lâmpadas simultaneamente...");
    for (int i = 0; i < 4; i++) {
        digitalWrite(relayPins[i], RELAY_ON);
    }
    delay(2000);

    Serial.println("[TESTE COLETIVO] Desligando todas as 4 lâmpadas...");
    for (int i = 0; i < 4; i++) {
        digitalWrite(relayPins[i], RELAY_OFF);
    }

    Serial.println("Aguardando 3 segundos para reiniciar ciclo...");
    delay(3000);
}

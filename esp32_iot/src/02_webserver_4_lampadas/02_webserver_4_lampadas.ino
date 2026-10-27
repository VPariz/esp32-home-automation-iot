/*
 * ============================================================================
 * Projeto: Laboratório de Redes I - Automação Residencial com ESP32
 * Roteiro 2: Servidor Web HTTP Wi-Fi para Controle de 4 Lâmpadas (Relés)
 * Plataforma: ESP32 DevKit V1 (30 pinos) + Módulo Relés Optoacoplados
 * Domínio: Internet das Coisas (IoT) & Sistemas Embarcados
 * ============================================================================
 *
 * Arquitetura de Rede:
 *   - Modo Dual: Conecta-se a um Ponto de Acesso existente (STA) ou, caso
 *     a rede do laboratório esteja indisponível, cria sua própria rede Wi-Fi (SoftAP).
 *   - Porta TCP: 80 (HTTP padrão).
 *   - Interface: Dashboard Web responsivo compatível com navegadores móveis e desktop.
 *
 * Mapeamento de GPIOs (Active-LOW):
 *   - IN1 -> GPIO 25 (Lâmpada 1 - Sala)
 *   - IN2 -> GPIO 26 (Lâmpada 2 - Quarto)
 *   - IN3 -> GPIO 32 (Lâmpada 3 - Cozinha)
 *   - IN4 -> GPIO 33 (Lâmpada 4 - Varanda)
 */

#include <WiFi.h>

// --- Configurações da Rede Wi-Fi ---
// Configure o SSID e a senha da sua rede local (ou hotspot)
const char* sta_ssid     = "SUA_REDE_WIFI"; 
const char* sta_password = "SUA_SENHA_WIFI";

// Configurações do Ponto de Acesso Próprio (Fallback SoftAP)
const char* ap_ssid      = "ESP32-Automacao";
const char* ap_password  = "senha_ponto_acesso"; // Mínimo 8 caracteres

// --- Pinos e Lógica ---
#define RELAY_1 25
#define RELAY_2 26
#define RELAY_3 32
#define RELAY_4 33

#define RELAY_ON  LOW
#define RELAY_OFF HIGH

const int relayPins[4] = {RELAY_1, RELAY_2, RELAY_3, RELAY_4};
const char* lampNames[4] = {"Sala", "Quarto", "Cozinha", "Varanda"};
bool relayStates[4] = {false, false, false, false}; // false = Apagada, true = Acesa

// Servidor Web na porta TCP 80
WiFiServer server(80);

void setRelay(int index, bool turnOn) {
    if (index >= 0 && index < 4) {
        relayStates[index] = turnOn;
        digitalWrite(relayPins[index], turnOn ? RELAY_ON : RELAY_OFF);
        Serial.printf("[COMANDO] %s: %s (Nível GPIO %d)\n", 
                      lampNames[index], turnOn ? "LIGADA" : "DESLIGADA", 
                      turnOn ? RELAY_ON : RELAY_OFF);
    }
}

void setAllRelays(bool turnOn) {
    for (int i = 0; i < 4; i++) {
        setRelay(i, turnOn);
    }
}

void sendHtmlResponse(WiFiClient& client) {
    // Cabeçalho HTTP
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html; charset=UTF-8");
    client.println("Connection: close");
    client.println();

    // Página HTML com CSS moderno e responsivo
    client.println("<!DOCTYPE html><html lang='pt-BR'><head>");
    client.println("<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>");
    client.println("<title>ESP32 IoT Home Automation Server</title>");
    client.println("<style>");
    client.println("body { font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif; background-color: #0f172a; color: #f8fafc; margin: 0; padding: 20px; display: flex; flex-direction: column; align-items: center; }");
    client.println(".container { max-width: 600px; width: 100%; }");
    client.println(".header { text-align: center; margin-bottom: 24px; padding: 15px; background: #1e293b; border-radius: 12px; box-shadow: 0 4px 6px rgba(0,0,0,0.3); }");
    client.println(".header h1 { font-size: 1.5rem; margin: 0 0 8px 0; color: #38bdf8; }");
    client.println(".header p { margin: 0; font-size: 0.9rem; color: #94a3b8; }");
    client.println(".grid { display: grid; grid-template-columns: 1fr 1fr; gap: 16px; margin-bottom: 20px; }");
    client.println(".card { background: #1e293b; padding: 16px; border-radius: 12px; text-align: center; border: 1px solid #334155; }");
    client.println(".card h3 { margin: 0 0 10px 0; font-size: 1.1rem; }");
    client.println(".status { display: inline-block; padding: 4px 10px; border-radius: 20px; font-weight: bold; font-size: 0.85rem; margin-bottom: 12px; }");
    client.println(".status.on { background: #166534; color: #4ade80; }");
    client.println(".status.off { background: #451a03; color: #f87171; }");
    client.println(".btn { display: block; width: 100%; padding: 10px 0; border: none; border-radius: 8px; font-weight: bold; cursor: pointer; text-decoration: none; text-align: center; font-size: 0.95rem; }");
    client.println(".btn-on { background: #0284c7; color: white; margin-bottom: 6px; }");
    client.println(".btn-on:hover { background: #0369a1; }");
    client.println(".btn-off { background: #475569; color: #cbd5e1; }");
    client.println(".btn-off:hover { background: #334155; }");
    client.println(".master-controls { display: flex; gap: 12px; margin-top: 10px; }");
    client.println(".btn-master-on { background: #22c55e; color: #052e16; flex: 1; }");
    client.println(".btn-master-off { background: #ef4444; color: white; flex: 1; }");
    client.println("</style></head><body>");

    client.println("<div class='container'>");
    client.println("<div class='header'>");
    client.println("<h1>ESP32 IoT Automation Hub</h1>");
    client.println("<p>Controle de Iluminação Residencial & Automação Industrial</p>");
    client.println("</div>");

    client.println("<div class='grid'>");
    for (int i = 0; i < 4; i++) {
        client.println("<div class='card'>");
        client.printf("<h3>Lâmpada %d - %s</h3>", i + 1, lampNames[i]);
        if (relayStates[i]) {
            client.println("<span class='status on'>💡 ACESA</span>");
            client.printf("<a class='btn btn-off' href='/lamp%d/off'>Desligar</a>", i + 1);
        } else {
            client.println("<span class='status off'>⚫ APAGADA</span>");
            client.printf("<a class='btn btn-on' href='/lamp%d/on'>Ligar</a>", i + 1);
        }
        client.println("</div>");
    }
    client.println("</div>");

    client.println("<div class='master-controls'>");
    client.println("<a class='btn btn-master-on' href='/all/on'>Ligar Todas</a>");
    client.println("<a class='btn btn-master-off' href='/all/off'>Desligar Todas</a>");
    client.println("</div>");

    client.println("</div></body></html>");
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n--- Inicializando ESP32 Web Server 4 Relés ---");

    // Configuração dos pinos dos relés (Active-LOW, desligados de início)
    for (int i = 0; i < 4; i++) {
        digitalWrite(relayPins[i], RELAY_OFF);
        pinMode(relayPins[i], OUTPUT);
        digitalWrite(relayPins[i], RELAY_OFF);
    }

    // Tentativa de conexão ao Wi-Fi Station (STA)
    Serial.printf("Tentando conectar a rede Wi-Fi: %s ...\n", sta_ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(sta_ssid, sta_password);

    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < 15) {
        delay(500);
        Serial.print(".");
        tentativas++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n[Wi-Fi STA Conectado com Sucesso!]");
        Serial.print("Endereço IP na rede: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\n[Wi-Fi STA Falhou] Ativando Modo Ponto de Acesso (SoftAP)...");
        WiFi.mode(WIFI_AP);
        WiFi.softAP(ap_ssid, ap_password);
        Serial.print("Rede Wi-Fi criada: ");
        Serial.println(ap_ssid);
        Serial.print("Endereço IP do ESP32 (SoftAP): ");
        Serial.println(WiFi.softAPIP());
    }

    // Inicia o servidor HTTP
    server.begin();
    Serial.println("[Servidor HTTP iniciado na porta 80]");
}

void loop() {
    WiFiClient client = server.available();
    if (!client) return;

    String currentLine = "";
    String requestHeader = "";
    unsigned long timeout = millis() + 2000;

    while (client.connected() && millis() < timeout) {
        if (client.available()) {
            char c = client.read();
            requestHeader += c;

            if (c == '\n') {
                if (currentLine.length() == 0) {
                    // Fim do cabeçalho da requisição HTTP -> processar rotas
                    if (requestHeader.indexOf("GET /lamp1/on") >= 0) setRelay(0, true);
                    else if (requestHeader.indexOf("GET /lamp1/off") >= 0) setRelay(0, false);
                    else if (requestHeader.indexOf("GET /lamp2/on") >= 0) setRelay(1, true);
                    else if (requestHeader.indexOf("GET /lamp2/off") >= 0) setRelay(1, false);
                    else if (requestHeader.indexOf("GET /lamp3/on") >= 0) setRelay(2, true);
                    else if (requestHeader.indexOf("GET /lamp3/off") >= 0) setRelay(2, false);
                    else if (requestHeader.indexOf("GET /lamp4/on") >= 0) setRelay(3, true);
                    else if (requestHeader.indexOf("GET /lamp4/off") >= 0) setRelay(3, false);
                    else if (requestHeader.indexOf("GET /all/on") >= 0) setAllRelays(true);
                    else if (requestHeader.indexOf("GET /all/off") >= 0) setAllRelays(false);

                    // Responde com o HTML atualizado
                    sendHtmlResponse(client);
                    break;
                } else {
                    currentLine = "";
                }
            } else if (c != '\r') {
                currentLine += c;
            }
        }
    }

    client.stop();
}

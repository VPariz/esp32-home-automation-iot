# Laboratório de Redes em C e IoT com ESP32

Exemplos práticos de programação de baixo nível com sockets de rede em linguagem C (TCP e UDP utilizando BSD Sockets e Winsock2) e automação residencial com microcontroladores ESP32.

---

## Estrutura do Repositório

```
lab-redes-c-esp32/
├── c_sockets/
│   ├── jogo_da_velha/
│   │   ├── servidor_velha.c             # Servidor TCP com controle de estado do jogo e turnos
│   │   └── cliente_velha.c              # Cliente interativo em terminal via TCP
│   ├── chat_udp/
│   │   ├── servidor-chat-2-usuarios-udp.c # Servidor de relay UDP entre dois clientes
│   │   └── cliente-chat-2-usuarios-udp.c  # Terminal assíncrono de mensagens
│   └── transferencia_arquivos_tcp/
│       ├── servidor-transf-arquivo-tcp.c # Receptor com gravação em disco e contagem de bytes
│       └── cliente-transf-arquivo-tcp.c  # Leitor e transmissor de payload binário via TCP
└── esp32_iot/
    ├── src/
    │   ├── 01_teste_reles/                  # Acionamento sequencial de relés de potência
    │   ├── 02_webserver_4_lampadas/         # Webserver HTTP embarcado com interface de controle
    │   └── 03_bluetooth_controle/           # Interface Bluetooth Serial (SPP) para controle remoto
    ├── TUTORIAL_GERAL_ESP32.tex             # Documento técnico em LaTeX sobre arquitetura e bancada
    └── TUTORIAL_GERAL_ESP32.pdf             # Guia técnico compilado (9 páginas)
```

---

## O que foi Desenvolvido

* **Programação de Redes em C:**
  * Uso direto das chamadas de sistema: `socket()`, `bind()`, `listen()`, `accept()`, `send()`, `recv()`, `sendto()` e `recvfrom()`.
  * **TCP:** Conexão confiável ponto a ponto para sincronização de jogadas e transmissão íntegra de arquivos binários entre máquinas.
  * **UDP:** Envio de datagramas sem conexão para bate-papo de baixa latência em rede local.
* **Firmware para ESP32:**
  * Servidor Web HTTP leve respondendo requisições com HTML e CSS guardados diretamente na memória flash.
  * Modo de contingência Wi-Fi: tenta se conectar ao roteador local (modo Station) e, se não encontrar, cria seu próprio ponto de acesso Wi-Fi (modo SoftAP).
  * Chaveamento de pinos GPIO para acionar relés optoacoplados controlando cargas de corrente alternada.
  * Comunicação sem fio via Bluetooth Classic para enviar comandos e receber leituras pelo celular.

---

## Como Compilar e Rodar os Códigos em C

### No Linux (GCC)

```bash
# Jogo da Velha (TCP)
gcc c_sockets/jogo_da_velha/servidor_velha.c -o servidor_velha
gcc c_sockets/jogo_da_velha/cliente_velha.c -o cliente_velha

# Em dois terminais separados:
./servidor_velha 8080
./cliente_velha 127.0.0.1 8080
```

### No Windows (MinGW / GCC)

No Windows, adicione a flag `-lws2_32` no final do comando para ligar a biblioteca de rede:

```bash
# Jogo da Velha (TCP)
gcc c_sockets/jogo_da_velha/servidor_velha.c -o servidor_velha.exe -lws2_32
gcc c_sockets/jogo_da_velha/cliente_velha.c -o cliente_velha.exe -lws2_32

# Chat UDP entre 2 usuários
gcc c_sockets/chat_udp/servidor-chat-2-usuarios-udp.c -o servidor_chat.exe -lws2_32
gcc c_sockets/chat_udp/cliente-chat-2-usuarios-udp.c -o cliente_chat.exe -lws2_32

# Transferência de Arquivos (TCP)
gcc c_sockets/transferencia_arquivos_tcp/servidor-transf-arquivo-tcp.c -o servidor_transf.exe -lws2_32
gcc c_sockets/transferencia_arquivos_tcp/cliente-transf-arquivo-tcp.c -o cliente_transf.exe -lws2_32
```

---

## Como Gravar os Códigos no ESP32

1. Abra a [Arduino IDE](https://www.arduino.cc/en/software).
2. Vá em **Arquivo $\rightarrow$ Preferências** e cole a URL de suporte ao ESP32:
   `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
3. No menu lateral de placas (**Boards Manager**), procure por **esp32** e instale.
4. Conecte o ESP32 ao computador pelo cabo USB e escolha a placa **DOIT ESP32 DEVKIT V1**.
5. Abra o arquivo `.ino` desejado dentro da pasta `esp32_iot/src/` e clique na seta para gravar (**Upload**).

---

## Licença
Distribuído sob licença MIT. Consulte o arquivo `LICENSE` para mais detalhes.

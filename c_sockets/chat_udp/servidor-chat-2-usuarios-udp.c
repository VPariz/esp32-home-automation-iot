#include <winsock2.h>

#include <windows.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define PORTA_SERV 27015

#define BUFLEN 512

int main() {        //  PROGRAMA SERVIDOR

    WSADATA wsaData;

    SOCKET s;

    struct sockaddr_in server_addr, client_addr1, client_addr2;

    int client1_addr_len = sizeof(client_addr1);

    int client2_addr_len = sizeof(client_addr2);

    char buf[BUFLEN];

    int recv_len;

    // Variáveis para o select()

    /* fd_set é uma estrutura de dados usada no Winsock

    para agrupar descritores de socket. Ela funciona com a função select()

    para determinar quais sockets estão prontos para operações de leitura e escrita */

    fd_set read_fds;

    // Inicialização do Winsock

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        printf("Falha na inicializacao do Winsock\n");

        return 1;

    }

    // Criação do socket UDP

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {

        printf("Falha na criacao do socket\n");

        return 1;

    }

    // Configuração do endereço do servidor

    memset((char *)&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(PORTA_SERV);

    server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

    // Vincula o socket ao endereço

    if (bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {

        printf("Falha no bind\n");

        return 1;

    }

    printf("Servidor UDP iniciado. Aguardando clientes...\n");

    // Receber a primeira mensagem do Cliente 1 para obter seu endereço

    recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&client_addr1, &client1_addr_len);

    printf("Cliente 1 conectado.\n");

    // Lógica para obter e armazenar o nome do cliente 1

    // Receber a primeira mensagem do Cliente 2 para obter seu endereço

    recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&client_addr2, &client2_addr_len);

    printf("Cliente 2 conectado. Chat iniciado.\n");

    // Lógica para obter e armazenar o nome do cliente 2

    // Loop principal do chat

    while (1) {

        FD_ZERO(&read_fds);

        FD_SET(s, &read_fds);

        // Monitora diversos sockets para ver se há dados disponíveis

        if (select(0, &read_fds, NULL, NULL, NULL) == SOCKET_ERROR) {

            printf("Erro no select\n");

            break;

        }

        // FD_ISSET() é uma macro usada com a função select() para verificar

        // se um soquete está pronto para leitura OU escrita

        if (FD_ISSET(s, &read_fds)) {

            // Recebe a mensagem e determina qual cliente a enviou

            struct sockaddr_in from_addr;

            int from_addr_len = sizeof(from_addr);

            recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&from_addr, &from_addr_len);

            // Se o remetente é o Cliente 1, envie a mensagem para o Cliente 2

            // memcmp() compara os n primeiros bytes de duas áreas de memória,

            // interpretando cada byte como um unsigned char

            if (memcmp(&from_addr, &client_addr1, from_addr_len) == 0) {

                sendto(s, buf, recv_len, 0, (struct sockaddr *)&client_addr2, client2_addr_len);

            }

            // Se o remetente é o Cliente 2, envie a mensagem para o Cliente 1

            else if (memcmp(&from_addr, &client_addr2, from_addr_len) == 0) {

                sendto(s, buf, recv_len, 0, (struct sockaddr *)&client_addr1, client1_addr_len);

            }

        }

    }

    // Limpeza da biblioteca da memoria

    closesocket(s);

    WSACleanup();

    return 0;

}


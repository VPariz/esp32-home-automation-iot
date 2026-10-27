#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORTA_SERV 27015

#define IP_SERV "127.0.0.1" // IP do servidor

#define BUFLEN 512

int main() {     //  PROGRAMA CLIENTE
    WSADATA wsaData;
    SOCKET s = INVALID_SOCKET;
    struct sockaddr_in server_addr;
    char buf[BUFLEN];
    char username[50];
    int server_addr_len = sizeof(server_addr);
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
    server_addr.sin_addr.S_un.S_addr = inet_addr(IP_SERV);
    printf("Digite seu nome: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0'; // Remove o \n
    // Envia o nome do usuário para o servidor
    sendto(s, username, strlen(username), 0, (struct sockaddr *)&server_addr, server_addr_len);
    printf("Chat iniciado. Digite 'sair' para encerrar.\n");
    while (1) {
        // Enviar mensagem
        printf("Voce: ");
        fgets(buf, BUFLEN, stdin);
        sendto(s, buf, strlen(buf), 0, (struct sockaddr *)&server_addr, server_addr_len);
        if (strcmp(buf, "sair\n") == 0) {
            break;
        }

        // Receber mensagem
        memset(buf, 0, BUFLEN);
        recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&server_addr, &server_addr_len);
        printf("Mensagem recebida: %s\n", buf);
    }

    // Fecha o socket, desaloca da memória a biblioteca
    closesocket(s);
    WSACleanup();
    return 0;
}


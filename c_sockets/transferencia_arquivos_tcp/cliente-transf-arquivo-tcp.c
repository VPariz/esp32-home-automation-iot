#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
// Programa cliente-tcp para transferencia de arquivo

#define REMOTE_SERVER_PORT 6000

#define TAM_BUFFER 1024

#define ARQUIVO_RECV "arquivo_recebido.txt" // Nome do arquivo a ser salvo

int main() {
    SOCKET sd;
    int rc;
    struct sockaddr_in servAddr;
    WSADATA wsaData;
    char recvbuf[TAM_BUFFER];
    char server_ip_str[30];
    FILE *f;
    int result;
    // A função WSAStartup é chamada para iniciar o uso de WS2_32.dll
    printf("\n Inicializando Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Falha na inicializacao \n");
        return EXIT_FAILURE;
    }

    printf("\n Digite o IP do servidor: ");
    if (fgets(server_ip_str, sizeof(server_ip_str), stdin) == NULL) {
        printf("Erro ao ler o IP.\n");
        WSACleanup();
        return EXIT_FAILURE;
    }

    server_ip_str[strcspn(server_ip_str, "\n")] = 0; // Remove o caractere de nova linha
    // Criação do socket TCP (SOCK_STREAM)
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == INVALID_SOCKET) {
        printf("Erro: Nao pode abrir o socket. Codigo de erro: %d\n", WSAGetLastError());
        WSACleanup();
        return EXIT_FAILURE;
    }

    printf("\n Abriu socket sd: %llu \n", (unsigned long long)sd);
    // Configurar o endereço do servidor
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(server_ip_str); // Usa inet_addr() para o IP
    servAddr.sin_port = htons(REMOTE_SERVER_PORT); // numero da porta
    // Conectar ao servidor
    printf("\n Conectando a %s:%u...\n", server_ip_str, REMOTE_SERVER_PORT);
    rc = connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (rc == SOCKET_ERROR) {
        printf("Erro ao conectar. Codigo de erro: %d\n", WSAGetLastError());
        closesocket(sd);
        WSACleanup();
        return 1;
    }

    printf("\n Conexao estabelecida. Preparando para receber o arquivo\n");
    // Abrir o arquivo
     f = fopen(ARQUIVO_RECV, "wb"); // Abre para escrita binária
     if (f == NULL) {
        printf("Erro ao criar arquivo para escrita: %s\n", ARQUIVO_RECV);
        closesocket(sd);
        WSACleanup();
        return 1;
    }

   // Receber e Salvar o Arquivo
    int totalBytesRecebidos = 0;
    do {
        result = recv(sd, recvbuf, TAM_BUFFER, 0);
        if (result > 0) {
            // Escreve os bytes recebidos no arquivo
            fwrite(recvbuf, 1, result, f);
            totalBytesRecebidos += result;
        } else if (result == 0) {
            // Conexão fechada pelo servidor (fim da transferência)
            printf("\n Conexao fechada pelo servidor.\n");
        } else {
            printf("Recv falhou: %d\n", WSAGetLastError());
        }

    } while (result > 0);
    fclose(f);
    printf("\n Recebimento concluido. Total de bytes recebidos: %d\n", totalBytesRecebidos);
    // Fechar a conexão
    closesocket(sd);
    WSACleanup();
    return 0;
}


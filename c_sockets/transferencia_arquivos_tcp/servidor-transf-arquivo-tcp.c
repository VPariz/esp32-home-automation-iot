#include <winsock2.h>

#include <windows.h>

#include <stdio.h>

#include <stdlib.h>

// Programa servidor-tcp para transferencia de arquivo

#define LOCAL_SERVER_PORT 6000

#define TAM_BUFFER 1024

#define ARQUIVO "arquivo_servidor.txt" // Nome do arquivo a ser enviado

#define TAM_LISTA   5   // Tamanho da lista de espera de clientes

int main(void) {

    // Socket de escuta (para aceitar novas conexões)

    SOCKET listen_sd, new_sd;

    int rc, cliLen;

    struct sockaddr_in cliAddr, servAddr;

    char BUFFER[TAM_BUFFER];

    FILE *f;


    WSADATA wsaData;

    printf("\n Inicializando Winsock...\n");

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {

        printf("Falha na inicializacao do Winsock: %d\n", WSAGetLastError());

        exit(1);

    }

    // Criação do socket TCP (SOCK_STREAM)

    listen_sd = socket(AF_INET, SOCK_STREAM, 0);

    if (listen_sd == INVALID_SOCKET) {

        printf("Nao pode abrir socket.\n");

        WSACleanup();

        exit(1);

    }

    printf("\n Abriu socket de escuta sd: %lld \n", listen_sd);

    // Associar (bind) a uma porta do servidor local

    servAddr.sin_family = AF_INET;

    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    servAddr.sin_port = htons(LOCAL_SERVER_PORT);

    rc = bind(listen_sd, (struct sockaddr *)&servAddr, sizeof(servAddr));

    if (rc < 0) {

        printf("Nao pode associar a porta do servidor %d.\n", LOCAL_SERVER_PORT);

        closesocket(listen_sd);

        WSACleanup();

        exit(1);

    }

    // Colocar o socket em modo de escuta (listen)

    rc = listen(listen_sd, TAM_LISTA);

    if (rc == SOCKET_ERROR) {

        printf("Erro ao iniciar a escuta. \n");

        closesocket(listen_sd);

        WSACleanup();

        exit(1);

    }

  while(1)

  {

    printf("\n Esperando por conexoes na porta TCP %u\n", LOCAL_SERVER_PORT);

    cliLen = sizeof(cliAddr);

    //Aceitar a conexão do cliente (accept)

    new_sd = accept(listen_sd, (struct sockaddr *)&cliAddr, &cliLen);

    if (new_sd == INVALID_SOCKET) {

        printf("Erro ao aceitar a conexao. ");

        return 1;

    }

    printf("\n Conexao aceita de %s:%u\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

    // -   Enviar o arquivo ---

    printf("\n Abrindo arquivo para envio: %s\n", ARQUIVO);

    f = fopen(ARQUIVO, "rb"); // Abre para leitura binária

    if (f == NULL)      {

            printf("Erro ao abrir arquivo.\n");

            return 1;

        }

    printf("\n Iniciando transferencia...\n");

    while (!feof(f)) {

        // Lê um bloco do arquivo

        size_t bytesRead = fread(BUFFER, 1, TAM_BUFFER, f);

        if (bytesRead > 0) {

        // Envia o bloco lido

        if (send(new_sd, BUFFER, (int)bytesRead, 0) < 0) {

                    printf("Send falhou: %d\n", WSAGetLastError());

                    fclose(f);

            }

        }

    }

    printf("\n Transferencia do arquivo concluida.\n");

    fclose(f);

    // Fecha o socket do cliente

    closesocket(new_sd);

  }

  // Fecha o socket de escuta e finaliza o Winsock (nunca alcançado neste loop)

    closesocket(listen_sd);

    WSACleanup();

}


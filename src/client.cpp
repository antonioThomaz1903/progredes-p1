#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>  // Para `inet_pton`
#include <unistd.h>
#include <string>
#include <cstdlib>
#include "../lib/server.h"

using std::string;

int main(int argc, char *argv[]) {
    // Criando o socket (AF_INET para IPv4, SOCK_STREAM para TCP)
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Erro ao criar o socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configurando endereco do server
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convertendo endereço IP do texto para binário
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Endereço inválido ou não suportado" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Conectando ao servidor
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Erro ao conectar" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Conectado ao servidor" << std::endl;

    // Receber uma mensagem do servidor
    
    string buffer(1024, '\0');
    string escolha;

    char letra;
    
    read(sock, &buffer[0], buffer.length() - 1);
    letra = buffer[0];
    
    std::cout << "Voce eh o jogador  " << buffer << std::endl;
    buffer.clear();

    // ---------- JOGO ----------


    // BUFFER = JOGADOR DA VEZ
    read(sock, &buffer[0], buffer.length() - 1);

    while(buffer != "end"){
        buffer.clear();

        // Receber status do mapa
        read(sock, &buffer[0], buffer.length() - 1);

        if(buffer[0] == letra){
            // Enviar posicao
            do{
                std::cout << "Escolha a posicao: ";
                std::cin >> escolha;
                send(sock, &escolha[0], escolha.length(), 0);
                buffer.clear();
                read(sock, &buffer[0], buffer.length() - 1);
                if(buffer != "ok!"){
                    std::cout << "Posicao invalida!" << std::endl;
                }
            }while(buffer != "ok!");

            // Receber status do mapa



        }
        
    }
    
    // Fechar o socket
    close(sock);

    exit(EXIT_SUCCESS);
}

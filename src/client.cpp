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
    string ack = "ack";
    // BEM VINDO
    read(sock, &buffer[0], 18);
    send(sock, &ack[0], 3, 0);
    std::cout << buffer << std::endl;
    

    buffer.clear();
    read(sock, &buffer[0], 1);
    
    letra = buffer[0];
    std::cout << "Voce eh o jogador " << letra << std::endl;
    send(sock, &ack[0], 3, 0);

    // ---------- JOGO ----------

    // MAPA
    buffer.clear();
    read(sock, &buffer[0], 30);
    send(sock, &ack[0], 3, 0);
    std::cout << buffer.c_str();
    

    // BUFFER = JOGADOR DA VEZ
    buffer.clear();
    buffer.assign(1024, '\0');
    read(sock, &buffer[0], 1);
    send(sock, &ack[0], 3, 0);
    std::cout << buffer.c_str() << std::endl;

    while(buffer != "end"){   

        if(buffer[0] == letra){
            // Enviar posicao
            do{
                std::cout << "Escolha a posicao: ";
                std::cin >> escolha;
                send(sock, &escolha[0], 3, 0);
                buffer.assign(1024, '\0');

                read(sock, &buffer[0], 3);

                buffer.assign(1024, '\0');
                read(sock, &buffer[0], 3);
                std::cout << buffer.c_str() << std::endl;
                if(buffer[0] != '1'){
                    std::cout << "Posicao invalida!" << std::endl;
                }

                buffer.assign(1024, '\0');
                read(sock, &buffer[0], 30);
                send(sock, &ack[0], 3, 0);

            }while(buffer[0] != '1');
        }
        else{
            std::cout << "Vez do adversario" << std::endl;
            buffer.clear();
            read(sock, &buffer[0], 4);
            if(buffer == "end"){
                buffer.clear();
                read(sock, &buffer[0], 2);
                std::cout << "Jogador " << buffer.c_str() << " ganhou!" << std::endl;
                break;
            }
        }

        // MAPA
        buffer.clear();
        read(sock, &buffer[0], buffer.length() - 1);
        std::cout << buffer;

        // JOGADOR DA VEZ
        buffer.clear();
        read(sock, &buffer[0], buffer.length() - 1);
    }
    
    // Fechar o socket
    close(sock);

    exit(EXIT_SUCCESS);
}

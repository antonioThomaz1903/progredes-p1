#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string>
#include "../lib/game.h"
#include "../lib/server.h"

using std::string;

int main(int argc, char *argv[]){

    // Definindo socket do servidor
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Erro ao criar o socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configuração do endereço do servidor
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;          // IPV4
    address.sin_addr.s_addr = INADDR_ANY;  // Aceita conexões de qualquer endereço IP
    address.sin_port = htons(PORT);        // Define a porta

    // Vinculando o socket ao endereço e porta
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Erro ao fazer o bind" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Colocando o socket em modo de escuta
    if (listen(server_fd, 2) < 0) {
        std::cerr << "Erro ao colocar em escuta" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor em escuta na porta " << PORT << std::endl;
    jogador jogador1(1, 'X');
    jogador jogador2(1, 'O');
    string buffer_send(1024, '\0');
    string buffer_read(1024, '\0');
    // Aceitando conexao client1

    int player1fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (player1fd < 0) {
        std::cerr << "Erro ao aceitar a conexão" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    string message("Bem-vindo ao jogo!");

    // Enviar uma mensagem de boas vindas para o cliente1
    send(player1fd, &message[0], message.length(), 0);
    read(player1fd, &buffer_read[0], 3);
    std::cout << buffer_read << " cliente 1 boas vindas" << std::endl;
    
    buffer_read.clear();
    
    buffer_send = jogador1.getLetra();
    
    send(player1fd, &buffer_send[0], 1, 0);
    buffer_send.clear();

    read(player1fd, &buffer_read[0], 3);
    
    std::cout << buffer_read.c_str() << " cliente 1 jogador x" << std::endl;

    buffer_read.clear();
    

    // Aceitando conexao client2
    int player2fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (player2fd < 0) {
        std::cerr << "Erro ao aceitar a conexão" << std::endl;
        close(player1fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Enviar uma mensagem de boas vindas para o cliente2
    send(player2fd, &message[0], message.length(), 0);

    buffer_send.clear();
    read(player2fd, &buffer_read[0], 3);
    std::cout << buffer_read.c_str() << " cliente 2 - boas vindas" << std::endl;
    
    buffer_read.clear();
    buffer_send = jogador2.getLetra();
    send(player2fd, &buffer_send[0], 1, 0);

    buffer_read.clear();
    read(player2fd, &buffer_read[0], 3);
    std::cout << buffer_read.c_str() << " cliente 2 jogador o" << std::endl;

    // ----------- JOGO -----------

    int tamanho = 3;
    jogo jogao(tamanho, jogador1, jogador2);


    buffer_read.clear();
    jogao.iniciaJogo(player1fd, player2fd);


    // Fechar sockets
    close(player1fd);
    close(player2fd);
    close(server_fd);

    exit(EXIT_SUCCESS);
}

void jogo::iniciaJogo(int jogador1fd, int jogador2fd) {
    string buffer(1024, '\0');
    string mapa;
    string letra;
    int posicaoEscolhida;
    while (status) {
        mapa = retornaMapa();
        send(jogador1fd, &mapa[0], mapa.length(), 30);
        send(jogador2fd, &mapa[0], mapa.length(), 30);

        // ACK MAPA P1
        read(jogador1fd, &buffer[0], 3);
        std::cout << buffer.c_str() << " p1 mapa" << "\n";
        buffer.clear();

        // ACK MAPA P2
        read(jogador2fd, &buffer[0], 3);
        std::cout << buffer.c_str() << " p2 mapa" << "\n";
        buffer.clear();

        std::cout << mapa.c_str();

        // Alterna entre os jogadores
        jogador& atual = (turno % 2 != 0) ? jogador1 : jogador2;
        
        std::cout << "Vez do jogador " << atual.getLetra() << std::endl;

        letra = atual.getLetra();
        std::cout << "VEZ DE " <<  letra << "\n";
        send(jogador1fd, &letra[0], 1, 0);
        send(jogador2fd, &letra[0], 1, 0);

        // ACK VEZ P1
        read(jogador1fd, &buffer[0], 3);
        std::cout << buffer.c_str() << " p1 vez" << "\n";
        buffer.clear();

        // ACK VEZ P1
        read(jogador2fd, &buffer[0], 3);
        std::cout << buffer.c_str() << " p1 vez" << "\n";
        buffer.clear();
        
        // Verifica jogador e le a jogada
        if(atual.getLetra() == jogador1.getLetra()){
            read(jogador1fd, &buffer[0], 3);
            send(jogador1fd, "ack\0", 4, 0);
        }
        else{
            read(jogador2fd, &buffer[0], 1);
            send(jogador2fd, "ack\0", 4, 0);
        }

        posicaoEscolhida = std::stoi(buffer);
        
        if (posicao[posicaoEscolhida - 1] == '-') {
            posicao[posicaoEscolhida - 1] = atual.getLetra();
            checaMapa();
            send(jogador1fd, "1\0", 4, 0);
            send(jogador2fd, "1\0", 4, 0);
            turno++;
        } else {
            std::cout << "Posição inválida. Tente novamente, jogador " << atual.getLetra() << std::endl;
            if(atual.getLetra() == jogador1.getLetra()){
                buffer.clear();
                send(jogador1fd, "0\0", 4, 0);
                read(jogador1fd, &buffer[0], 3);
                std::cout << buffer.c_str() << " jogador1 jogada" << std::endl;
            }
            else{
                buffer.clear();
                send(jogador2fd, "0\0", 4, 0);
                read(jogador2fd, &buffer[0], 3);
                std::cout << buffer.c_str() << " jogador1 jogada" << std::endl;
            }
        }
        if (!status) 
            std::cout << "Jogo encerrado: Jogador " << atual.getLetra() << " Ganhou!!!!!" << std::endl;
            send(jogador1fd, "2\0", 4, 0);
            send(jogador2fd, "2\0", 4, 0);
            send(jogador1fd, &letra[0], letra.length(), 0);
            send(jogador2fd, &letra[0], letra.length(), 0);
            break;
        buffer.clear();
    }

    std::cout << retornaMapa();
}
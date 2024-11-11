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

    // Aceitando conexao client1
    int player1fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (player1fd < 0) {
        std::cerr << "Erro ao aceitar a conexão" << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Aceitando conexao client2
    int player2fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (player2fd < 0) {
        std::cerr << "Erro ao aceitar a conexão" << std::endl;
        close(player1fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    

    // ----------- JOGO -----------

    int tamanho = 3;
    jogador jogador1(1, 'X');
    jogador jogador2(1, 'O');
    jogo jogao(tamanho, jogador1, jogador2);

    string buffer_aux(1, jogador1.getLetra());
    // Enviar uma mensagem de boas vindas para o cliente1
    string message("Bem-vindo ao jogo!");
    send(player1fd, &message[0], message.length(), 0);
    send(player1fd, &buffer_aux[0], buffer_aux.length(), 0);
    std::cout << "Mensagem enviada para o cliente 1" << std::endl;
    buffer_aux.clear();
    buffer_aux = jogador2.getLetra();

    // Enviar uma mensagem de boas vindas para o cliente2
    send(player2fd, &message[0], message.length(), 0);
    send(player2fd, &buffer_aux[0], buffer_aux.length(), 0);
    std::cout << "Mensagem enviada para o cliente 2" << std::endl;

    buffer_aux.clear();
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
        send(jogador1fd, &mapa[0], mapa.length(), 0);
        send(jogador2fd, &mapa[0], mapa.length(), 0);
        
        std::cout << mapa;
        // Alterna entre os jogadores
        jogador& atual = (turno % 2 != 0) ? jogador1 : jogador2;
        
        std::cout << "Vez do jogador " << atual.getLetra() << std::endl;

        letra = atual.getLetra();
        send(jogador1fd, &letra[0], letra.length(), 0);
        send(jogador2fd, &letra[0], letra.length(), 0);
        
        // Verifica jogador e le a jogada
        if(atual.getLetra() == jogador1.getLetra()){
            if(read(jogador1fd, &buffer[0], 1) <= 0){
                std::cerr << "Falha na leitura do jogador " << atual.getLetra() << std::endl;
                send(jogador1fd, "err\0", 4, 0);
                continue;
            }
            send(jogador1fd, "ok!\0", 4, 0);
        }
        else{
            if(read(jogador2fd, &buffer[0], 1) <= 0){
                std::cerr << "Falha na leitura do jogador " << atual.getLetra() << std::endl;
                send(jogador2fd, "err\0", 4, 0);
                continue;
            }
            send(jogador2fd, "ok!\0", 4, 0);
        }

        posicaoEscolhida = std::stoi(buffer);
        
        if (posicao[posicaoEscolhida - 1] == '-') {
            posicao[posicaoEscolhida - 1] = atual.getLetra();
            checaMapa();
            turno++;
        } else {
            std::cout << "Posição inválida. Tente novamente, jogador " << atual.getLetra() << std::endl;
            if(atual.getLetra() == jogador1.getLetra())
                send(jogador1fd, "err\0", 4, 0);
            else
                send(jogador2fd, "err\0", 4, 0);
        }
        if (!status) 
            std::cout << "Jogo encerrado: Jogador " << atual.getLetra() << " Ganhou!!!!!" << std::endl;
            send(jogador1fd, "end\0", 4, 0);
            send(jogador2fd, "end\0", 4, 0);

        buffer.clear();
    }

    std::cout << retornaMapa();
}
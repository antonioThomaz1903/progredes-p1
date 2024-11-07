#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../lib/game.h"

jogador::jogador(int ultimoTurno, char letra) : ultimoTurno(ultimoTurno), letra(letra) {}

int jogador::getTurno() const {
    return ultimoTurno;
}

void jogador::setTurno(int turno) {
    ultimoTurno = turno;
}

char jogador::getLetra() const {
    return letra;
}

jogo::jogo(int tamanho, jogador jogador1, jogador jogador2)
    : tamanho(tamanho), status(true), turno(1), jogador1(jogador1), jogador2(jogador2) {
    posicao = new char[tamanho * tamanho];
    iniciaMapa();
}

jogo::~jogo() {
    delete[] posicao;
}

void jogo::iniciaMapa() {
    system("clear");
    for (int i = 0; i < tamanho * tamanho; i++) {
        posicao[i] = '-';
    }
}

void jogo::retornaMapa() const {
    int aux = 0;
    while (aux < tamanho * tamanho) {
        printf("%c", posicao[aux]);
        aux++;
        if(aux % tamanho != 0) {
            printf("|");
        } else {
            printf("\n");
            // for(int i = 0; i < (tamanho * 2) - 1; i++) {
            //     printf("-");
            // }
            // printf("\n");
        }
    }
}

void jogo::checaMapa() {
    if(checaColuna() || checaDiagonal() || checaLinha())
        status = false; 
}

bool jogo::checaColuna() const {
    for (int i = 0; i < tamanho; i++) {
        char elementoinicial = posicao[i];
        if (elementoinicial != '-') {
            bool venceu = true;
            for (int j = 1; j < tamanho; j++) {
                if (posicao[j * tamanho + i] != elementoinicial) {
                    venceu = false;
                    break;
                }
            }
            if (venceu) return true;
        }
    }
    return false;
}

bool jogo::checaDiagonal() const {
    char elementoinicial = posicao[0];
    if (elementoinicial != '-') {
        bool venceu = true;
        for (int i = 1; i < tamanho; i++) {
            if (posicao[i * tamanho + i] != elementoinicial) {
                venceu = false;
                break;
            }
        }
        if (venceu) return true;
    }

    elementoinicial = posicao[tamanho - 1];
    if (elementoinicial != '-') {
        bool venceu = true;
        for (int i = 1; i < tamanho; i++) {
            if (posicao[i * tamanho + (tamanho - 1 - i)] != elementoinicial) {
                venceu = false;
                break;
            }
        }
        if (venceu) return true;
    }

    return false;
}

bool jogo::checaLinha() const {
    for (int i = 0; i < tamanho; i++) {
        char elementoinicial = posicao[i * tamanho];
        if (elementoinicial != '-') {
            bool venceu = true;
            for (int j = 1; j < tamanho; j++) {
                if (posicao[i * tamanho + j] != elementoinicial) {
                    venceu = false;
                    break;
                }
            }
            if (venceu) return true;
        }
    }
    return false;
}

void jogo::iniciaJogo() {
    while (status) {
        retornaMapa();
        
        // Alterna entre os jogadores
        jogador& atual = (turno % 2 != 0) ? jogador1 : jogador2;
        int posicaoEscolhida;
        
        printf("Jogador %c, escolha uma posição: ", atual.getLetra());
        std::cin >> posicaoEscolhida;
        
        if (posicao[posicaoEscolhida - 1] == '-') {
            posicao[posicaoEscolhida - 1] = atual.getLetra();
            checaMapa();
            turno++;
        } else {
            printf("Posição inválida. Tente novamente.\n");
        }
        if (!status) 
            printf("Jogo encerrado: Jogador %c Ganhou!!!!!\n", atual.getLetra());
    }
    retornaMapa();

}

int main (int argc, char *argv[]) {
    if (std::atoi(argv[1]) < 1) {
        printf("Tamanho Invalido. Tente novamente.\n");
        exit(0);
    }
    int tamanho = std::atoi(argv[1]);
    jogador jogador1(1, 'X');
    jogador jogador2(1, 'O');
    jogo jogao(tamanho, jogador1, jogador2);
    jogao.iniciaJogo();

    return 0;
}
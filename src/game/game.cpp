#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game.h"

jogo::jogo(int tamanho) : tamanho(tamanho), status(true), turno(1) {
    posicao = new char[tamanho * tamanho];
    iniciaMapa();
}

jogo::~jogo() {
    delete[] posicao;
}

void jogo::iniciaMapa() {

    system("clear");

    if (posicao == nullptr) {
        printf("Erro.");
        exit(1);
    }
    for (int i = 0; i < tamanho * tamanho; i++) {
        posicao[i] = '-';
    }
}

void jogo::retornaMapa() const {
    int aux;
    while (aux < tamanho * tamanho) {
        printf("%c", posicao[aux]);
        aux++;
        if(aux%tamanho != 0) {
            printf("|");
        } else {
            printf("\n");
            for(int i = 0; i < (tamanho * 2) - 1; i++) {
                printf("-");
            }
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
            if (venceu) {
                return true;
            }
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
        if (venceu) {
            return true;
        }
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
        if (venceu) {
            return true;
        }
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
            if (venceu) {
                return true;
            }
        }
    }
    return false;
}
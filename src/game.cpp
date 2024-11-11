#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string>
#include "../lib/game.h"
#include "../lib/server.h"
using std::string;

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

string jogo::retornaMapa() const {
    int aux = 0;
    string mapa_str(1, '\0');
    string letra;
    while (aux < tamanho * tamanho) {
        letra = posicao[aux];
        mapa_str.append(letra);
        aux++;
        if(aux % tamanho != 0) {
            mapa_str.append("|");
        } else {
            mapa_str.append("\n");
        }
    }
    return mapa_str;
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


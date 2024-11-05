#ifndef JOGO_H
#define JOGO_H

#include <iostream>

class jogo {
    private:
        int tamanho; // número de colunas ou linhas
        char *posicao; // armazena os X, O e espaços vazios
        bool status; // indica o estado do jogo
        int turno; // indica o turno atual para alternar entre jogadores

        bool checaColuna() const;
        bool checaLinha() const;
        bool checaDiagonal() const;

    public:
        jogo(int tamanho);
        ~jogo();

        void iniciaMapa();
        void retornaMapa() const;
        void checaMapa();
        void jogada(int posicao, char simbolo);
        void iniciaJogo();
};


class jogador {
    int ultimoTurno;
};
#endif

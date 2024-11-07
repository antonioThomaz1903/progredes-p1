#ifndef GAME_H
#define GAME_H

#include <iostream>

class jogador {
    private:
        int ultimoTurno;
        char letra;
    public:
        jogador(int ultimoTurno = 0, char letra = 'X');
        ~jogador() = default;
        void setTurno(int turno);
        int getTurno() const;
        char getLetra() const;
};

class jogo {
    private:
        int tamanho;
        char *posicao;
        bool status;
        int turno;
        jogador jogador1;
        jogador jogador2;

        bool checaColuna() const;
        bool checaLinha() const;
        bool checaDiagonal() const;

    public:
        jogo(int tamanho, jogador jogador1, jogador jogador2);
        ~jogo();

        void iniciaMapa();
        void retornaMapa() const;
        void checaMapa();
        void iniciaJogo();
};

#endif

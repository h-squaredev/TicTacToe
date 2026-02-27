#ifndef TABULEIRO_H 
#define TABULEIRO_H 
#include <iostream>
#include <array>

namespace JogoDaVelha {

    class Tabuleiro{
        private:
            //decidi usar array porque a matriz nao precisa mudar de tamanho e gostei dela
            std::array<std::array<char, 3>, 3> matriz; //Matriz 3x3 fixa
            int numJogadas;
        public:
           //Construtor
            Tabuleiro();
            
            //Metodo obrigaorio
            bool aplicarJogada (int posicao, char simbolo);
            char verificarVencedor() const;
            bool isCheio() const;
            void imprimirTabuleiro() const;

    };
    
}

#endif
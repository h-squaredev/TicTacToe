#include "Tabuleiro.h"
#include <iostream>

namespace JogoDaVelha {
    //criando um CONSTRUTOR que inicia com espaçoes vazios e numJogadas em 0
    Tabuleiro::Tabuleiro() : numJogadas(0){
                for (auto &linha : matriz) linha.fill(' '); // ".fill" eh um metodo que preenche todos os elementos do conteiner
            }

    bool Tabuleiro::aplicarJogada(int posicao, char simbolo){
        if (posicao < 1 || posicao >9) {
            return false;
        }

        //COnversao da posicao 1-9 para indices [i][j] da matriz 3x3 (por causa da array)
        int i = (posicao - 1) / 3; //linha
        int j = (posicao - 1) % 3; //coluna

        //veridicacao se o espaco esta vazio
        if (matriz [i][j] != ' ') {
            return false;
        }

        //aplicando a jogada e incrementando o numero de jogadas
        matriz[i][j] = simbolo;
        numJogadas++;
        return true;
    }
    
    char Tabuleiro::verificarVencedor() const {
        //verificando se ganhou na linha ou coluna respectivamente
        for (int i = 0; i < 3; i++) {
            if (matriz[i][0] != ' ' && matriz[i][0] && matriz[i][0] == matriz[i][1] && matriz[i][1] == matriz[i][2]){
                return matriz[i][0];
            }
            if (matriz[0][i] != ' ' && matriz[0][i] == matriz[1][i] && matriz[1][i] == matriz[2][i]){
                return matriz[0][i];
            }
        }

        //verificando se ganhou nas diagonais
        if (matriz[0][0] != ' ' && matriz[0][0] == matriz[1][1] && matriz[1][1] == matriz[2][2]){
            return matriz[0][0];
        }
        if (matriz[0][2] != ' ' && matriz[0][2] == matriz[1][1] && matriz[1][1] == matriz[2][0]){
            return matriz[0][2];
        }
        return ' '; //retorna espaco vazio se nao tiver vencedor
    }

    bool Tabuleiro::isCheio() const {
        return numJogadas >= 9; //9 posicoes cheias = 9 jogadas completas, esta cheio
    }
    
    //imprime o tabuleiro de forma visual no terminal
    void Tabuleiro::imprimirTabuleiro() const {
        std::cout << "\n Jogada numero: " << numJogadas << "\n"; 

        for (int i = 0; i < 3; i++) {
            std::cout << " " << matriz[i][0] << " | " << matriz[i][1] << " | " << matriz[i][2] << " ";
            if (i < 2){
                std::cout << "\n-----------";
            }

            std::cout << "\n";
        }
    }
        
    
}
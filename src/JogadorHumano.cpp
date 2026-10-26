#include "JogadorHumano.h"

#include "Tabuleiro.h"

#include <iostream>
#include <limits>

// Construtor simples: delega para classe base.
JogadorHumano::JogadorHumano(const std::string& nome, char simbolo)
    : Jogador(nome, simbolo) {
}

// Le repetidamente ate obter jogada valida.
int JogadorHumano::escolherJogada(Tabuleiro& tabuleiro) {
    while (true) {
        std::cout << "Escolha uma posicao (1-9) ou 0 para suspender: ";
        int posicao;
        if (!(std::cin >> posicao)) {
            // Recupera estado do stream para continuar lendo entrada.
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. Tente novamente.\n";
            continue;
        }

        // Atalho para suspensao manual da partida.
        if (posicao == 0) {
            return 0;
        }

        if (posicao < 1 || posicao > 9) {
            std::cout << "Posicao fora do intervalo.\n";
            continue;
        }

        if (!tabuleiro.isLivre(posicao)) {
            std::cout << "Essa posicao ja esta ocupada.\n";
            continue;
        }

        return posicao;
    }
}

// Identificador persistente do tipo.
std::string JogadorHumano::tipo() const {
    return "humano";
}

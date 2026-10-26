#include "Tabuleiro.h"

#include <iostream>

// Construtor inicia estrutura e delega limpeza para reset().
Tabuleiro::Tabuleiro() : casas_{}, numJogadas_(0) {
    reset();
}

// Restaura tabuleiro vazio e contador de jogadas.
void Tabuleiro::reset() {
    casas_.fill(' ');
    numJogadas_ = 0;
}

// Aplica jogada valida: posicao dentro do range e casa livre.
bool Tabuleiro::aplicarJogada(int posicao, char simbolo) {
    if (posicao < 1 || posicao > 9) {
        return false;
    }
    if (!isLivre(posicao)) {
        return false;
    }

    casas_[static_cast<std::size_t>(posicao - 1)] = simbolo;
    ++numJogadas_;
    return true;
}

// Remove jogada (utilizado para simulacoes de IA).
bool Tabuleiro::removerJogada(int posicao) {
    if (posicao < 1 || posicao > 9) {
        return false;
    }

    std::size_t indice = static_cast<std::size_t>(posicao - 1);
    if (casas_[indice] == ' ') {
        return false;
    }

    casas_[indice] = ' ';
    --numJogadas_;
    return true;
}

// Avalia as 8 combinacoes possiveis de vitoria.
char Tabuleiro::verificarVencedor() const {
    static const int linhas[8][3] = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9},
        {1, 4, 7}, {2, 5, 8}, {3, 6, 9},
        {1, 5, 9}, {3, 5, 7}
    };

    for (const auto& linha : linhas) {
        char a = getCasa(linha[0]);
        char b = getCasa(linha[1]);
        char c = getCasa(linha[2]);
        if (a != ' ' && a == b && b == c) {
            return a;
        }
    }
    return ' ';
}

// Tabuleiro cheio implica empate quando nao houve vencedor.
bool Tabuleiro::isCheio() const {
    return numJogadas_ >= 9;
}

// Verifica disponibilidade da casa solicitada.
bool Tabuleiro::isLivre(int posicao) const {
    if (posicao < 1 || posicao > 9) {
        return false;
    }
    return casas_[static_cast<std::size_t>(posicao - 1)] == ' ';
}

// Le valor de uma casa; retorna vazio para posicao invalida.
char Tabuleiro::getCasa(int posicao) const {
    if (posicao < 1 || posicao > 9) {
        return ' ';
    }
    return casas_[static_cast<std::size_t>(posicao - 1)];
}

// Exposicao do contador para interface e logs.
int Tabuleiro::jogadasRealizadas() const {
    return numJogadas_;
}

// Coleta todas as posicoes ainda disponiveis.
std::vector<int> Tabuleiro::jogadasLivres() const {
    std::vector<int> livres;
    for (int pos = 1; pos <= 9; ++pos) {
        if (isLivre(pos)) {
            livres.push_back(pos);
        }
    }
    return livres;
}

// Formato: 9 chars, '_' para vazio e X/O ocupados.
std::string Tabuleiro::serializar() const {
    std::string estado;
    estado.reserve(9);
    for (char casa : casas_) {
        estado.push_back(casa == ' ' ? '_' : casa);
    }
    return estado;
}

// Restaura tabuleiro a partir da serializacao textual.
bool Tabuleiro::desserializar(const std::string& estado) {
    if (estado.size() != 9) {
        return false;
    }

    reset();
    for (std::size_t i = 0; i < estado.size(); ++i) {
        char c = estado[i];
        if (c == '_') {
            casas_[i] = ' ';
        } else if (c == 'X' || c == 'O') {
            casas_[i] = c;
            ++numJogadas_;
        } else {
            return false;
        }
    }
    return true;
}

// Impressao amigavel com indice nas casas vazias.
void Tabuleiro::imprimirTabuleiro() const {
    auto simboloOuIndice = [this](int pos) {
        char c = getCasa(pos);
        if (c == ' ') {
            return static_cast<char>('0' + pos);
        }
        return c;
    };

    std::cout << "\nTabuleiro:\n";
    for (int linha = 0; linha < 3; ++linha) {
        int base = linha * 3;
        std::cout << " " << simboloOuIndice(base + 1)
                  << " | " << simboloOuIndice(base + 2)
                  << " | " << simboloOuIndice(base + 3) << "\n";
        if (linha < 2) {
            std::cout << "-----------\n";
        }
    }
}

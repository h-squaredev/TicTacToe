#ifndef JOGADOR_COMPUTADOR_H
#define JOGADOR_COMPUTADOR_H

#include "Jogador.h"
#include <random>
#include <vector>

// Base comum para bots com utilitarios de IA reutilizaveis.
class JogadorComputador : public Jogador {
public:
    // Inicializa bot e semeia gerador aleatorio.
    JogadorComputador(const std::string& nome, char simbolo);

protected:
    // Escolhe uma posicao aleatoria dentre as disponiveis.
    int escolherAleatoria(const std::vector<int>& jogadas) const;

    // Simula jogadas para encontrar uma vitoria imediata.
    int encontrarJogadaVencedora(Tabuleiro& tabuleiro, char simboloTeste) const;

private:
    // RNG mutavel para uso em metodos const.
    mutable std::mt19937 rng_;
};

// Nivel 1: escolhe sempre aleatorio.
class BotAleatorio : public JogadorComputador {
public:
    BotAleatorio(const std::string& nome, char simbolo);
    int escolherJogada(Tabuleiro& tabuleiro) override;
    std::string tipo() const override;
};

// Nivel 2: tenta vencer; se nao der, aleatorio.
class BotBasico : public JogadorComputador {
public:
    BotBasico(const std::string& nome, char simbolo);
    int escolherJogada(Tabuleiro& tabuleiro) override;
    std::string tipo() const override;
};

// Nivel 3: tenta vencer; senao bloqueia; senao aleatorio.
class BotMedio : public JogadorComputador {
public:
    BotMedio(const std::string& nome, char simbolo);
    int escolherJogada(Tabuleiro& tabuleiro) override;
    std::string tipo() const override;
};

#endif

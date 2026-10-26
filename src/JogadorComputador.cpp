#include "JogadorComputador.h"

#include "Tabuleiro.h"

#include <algorithm>

// Construtor base de bots com RNG pronto para sorteios.
JogadorComputador::JogadorComputador(const std::string& nome, char simbolo)
    : Jogador(nome, simbolo), rng_(std::random_device{}()) {
}

// Seleciona jogada aleatoria entre as opcoes recebidas.
int JogadorComputador::escolherAleatoria(const std::vector<int>& jogadas) const {
    if (jogadas.empty()) {
        return -1;
    }
    std::uniform_int_distribution<int> dist(0, static_cast<int>(jogadas.size()) - 1);
    return jogadas[static_cast<std::size_t>(dist(rng_))];
}

// Testa cada jogada livre e verifica se gera vitoria imediata.
int JogadorComputador::encontrarJogadaVencedora(Tabuleiro& tabuleiro, char simboloTeste) const {
    for (int posicao : tabuleiro.jogadasLivres()) {
        tabuleiro.aplicarJogada(posicao, simboloTeste);
        bool venceu = (tabuleiro.verificarVencedor() == simboloTeste);
        tabuleiro.removerJogada(posicao);

        if (venceu) {
            return posicao;
        }
    }
    return -1;
}

// Bot nivel 1.
BotAleatorio::BotAleatorio(const std::string& nome, char simbolo)
    : JogadorComputador(nome, simbolo) {
}

// Sem estrategia: apenas sorteia.
int BotAleatorio::escolherJogada(Tabuleiro& tabuleiro) {
    return escolherAleatoria(tabuleiro.jogadasLivres());
}

std::string BotAleatorio::tipo() const {
    return "bot_aleatorio";
}

// Bot nivel 2.
BotBasico::BotBasico(const std::string& nome, char simbolo)
    : JogadorComputador(nome, simbolo) {
}

// Estrategia greedy: vence agora, senao sorteia.
int BotBasico::escolherJogada(Tabuleiro& tabuleiro) {
    int jogadaDeVitoria = encontrarJogadaVencedora(tabuleiro, simbolo_);
    if (jogadaDeVitoria != -1) {
        return jogadaDeVitoria;
    }
    return escolherAleatoria(tabuleiro.jogadasLivres());
}

std::string BotBasico::tipo() const {
    return "bot_basico";
}

// Bot nivel 3.
BotMedio::BotMedio(const std::string& nome, char simbolo)
    : JogadorComputador(nome, simbolo) {
}

// Prioriza: vencer > bloquear > sortear.
int BotMedio::escolherJogada(Tabuleiro& tabuleiro) {
    int jogadaDeVitoria = encontrarJogadaVencedora(tabuleiro, simbolo_);
    if (jogadaDeVitoria != -1) {
        return jogadaDeVitoria;
    }

    char simboloOponente = (simbolo_ == 'X') ? 'O' : 'X';
    int jogadaDeBloqueio = encontrarJogadaVencedora(tabuleiro, simboloOponente);
    if (jogadaDeBloqueio != -1) {
        return jogadaDeBloqueio;
    }

    return escolherAleatoria(tabuleiro.jogadasLivres());
}

std::string BotMedio::tipo() const {
    return "bot_medio";
}

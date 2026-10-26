#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <array>
#include <string>
#include <vector>

// Classe responsavel por encapsular o estado e as regras do tabuleiro 3x3.
class Tabuleiro {
public:
    // Construtor padrao: inicializa tabuleiro vazio.
    Tabuleiro();

    // Limpa o estado interno para iniciar nova partida.
    void reset();

    // Aplica uma jogada valida (1..9) se a casa estiver livre.
    bool aplicarJogada(int posicao, char simbolo);

    // Remove jogada de uma casa (usado por simulacoes da IA).
    bool removerJogada(int posicao);

    // Retorna 'X', 'O' ou ' ' quando nao existe vencedor.
    char verificarVencedor() const;

    // Indica se o tabuleiro esta cheio.
    bool isCheio() const;

    // Verifica se uma posicao esta disponivel para jogar.
    bool isLivre(int posicao) const;

    // Le o conteudo de uma casa especifica.
    char getCasa(int posicao) const;

    // Quantidade de jogadas aplicadas.
    int jogadasRealizadas() const;

    // Lista de posicoes livres, util para os bots.
    std::vector<int> jogadasLivres() const;

    // Converte tabuleiro para string de persistencia.
    std::string serializar() const;

    // Restaura tabuleiro a partir de string serializada.
    bool desserializar(const std::string& estado);

    // Exibe o tabuleiro no terminal.
    void imprimirTabuleiro() const;

private:
    // Estado fixo das 9 casas do jogo.
    std::array<char, 9> casas_;

    // Contador de jogadas para detectar empate rapidamente.
    int numJogadas_;
};

#endif

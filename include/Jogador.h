#ifndef JOGADOR_H
#define JOGADOR_H

#include <string>

class Tabuleiro;

// Classe abstrata que define o contrato comum para humano e bots.
class Jogador {
public:
    // Inicializa identidade e simbolo do jogador.
    Jogador(std::string nome, char simbolo);

    // Destrutor virtual obrigatorio para uso polimorfico seguro.
    virtual ~Jogador() = default;

    // Metodo polimorfico: cada classe derivada escolhe jogada de um jeito.
    virtual int escolherJogada(Tabuleiro& tabuleiro) = 0;

    // Identificador textual do tipo concreto (persistencia).
    virtual std::string tipo() const = 0;

    // Acesso somente leitura ao nome.
    const std::string& nome() const;

    // Retorna simbolo associado ao jogador.
    char simbolo() const;

protected:
    // Nome apresentado no menu/partida.
    std::string nome_;

    // Simbolo no tabuleiro ('X' ou 'O').
    char simbolo_;
};

#endif

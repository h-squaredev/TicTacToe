#ifndef JOGADOR_HUMANO_H
#define JOGADOR_HUMANO_H

#include "Jogador.h"

// Implementacao concreta para jogador controlado por entrada do teclado.
class JogadorHumano : public Jogador {
public:
    // Reaproveita estado base e define jogador humano.
    JogadorHumano(const std::string& nome, char simbolo);

    // Le entrada do usuario e valida jogada.
    int escolherJogada(Tabuleiro& tabuleiro) override;

    // Retorna identificador de tipo para persistencia.
    std::string tipo() const override;
};

#endif

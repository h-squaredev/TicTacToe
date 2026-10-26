#include "Jogador.h"

#include <utility>

// Construtor base para tipos concretos de jogador.
Jogador::Jogador(std::string nome, char simbolo)
    : nome_(std::move(nome)), simbolo_(simbolo) {
}

// Getter de nome.
const std::string& Jogador::nome() const {
    return nome_;
}

// Getter de simbolo.
char Jogador::simbolo() const {
    return simbolo_;
}

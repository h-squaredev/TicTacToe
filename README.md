# Jogo da Velha em C++ (POO)

Miniprojeto da disciplina de Programação II (UFMT). O projeto começou em dupla com Carlos Frazão (Carlos-Frazao), e os commits dele continuam no histórico. Esta é a versão completa, a mesma de Programming-II/Trabalho_JogoDaVelha. O enunciado está em Miniprojeto Prog2.pdf e o relatório em RELATORIO/.

## O que tem
- Orientação a objetos: encapsulamento, herança, classe abstrata e polimorfismo
- Jogador humano e três bots (aleatório, básico e médio)
- Menu com escolha de quem começa, estatísticas, top 10 e suspender/continuar a partida
- Ponteiros inteligentes (std::unique_ptr), sem new/delete manual

## Como rodar
Precisa de g++ com C++17 e make.

    make
    ./jogo_da_velha

Para limpar o que foi compilado: make clean

## Pastas
- src/ e include/: código
- data/: top 10 salvo em arquivo
- RELATORIO/: relatório em PDF

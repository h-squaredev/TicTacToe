#ifndef MOTOR_JOGO_H
#define MOTOR_JOGO_H

#include "Jogador.h"
#include "Tabuleiro.h"

#include <array>
#include <memory>
#include <string>
#include <vector>

// Estatisticas acumuladas da sessao atual para humano.
struct Estatisticas {
    int vitorias = 0;
    int empates = 0;
    int derrotas = 0;
};

// Niveis de IA disponiveis no menu.
enum class Dificuldade {
    Elementar = 1,
    Basico = 2,
    Medio = 3
};

// Configuracao de quem inicia a partida.
enum class Inicio {
    Humano = 1,
    Computador = 2,
    Aleatorio = 3
};

// Registro persistido no ranking Top 10.
struct RegistroRanking {
    std::string nome;
    int vitorias = 0;
    int empates = 0;
    int derrotas = 0;

    // Pontuacao usada na ordenacao do ranking.
    int pontos() const {
        return (vitorias * 3) + empates;
    }
};

// Classe principal que orquestra menu, partida, persistencia e ranking.
class MotorJogo {
public:
    // Configura estado inicial da aplicacao.
    MotorJogo();

    // Loop principal do menu.
    void executar();

private:
    // Renderiza opcoes do menu principal.
    void mostrarMenu() const;

    // Le e valida opcao numerica no intervalo informado.
    int lerOpcao(int minimo, int maximo) const;

    // Acao 1: iniciar novo jogo ou continuar jogo salvo.
    void acaoIniciarOuContinuar();

    // Acao 2: escolher quem inicia a proxima partida.
    void acaoEscolherQuemInicia();

    // Acao 3: alterar nivel de dificuldade do bot.
    void acaoModoFuncionamento();

    // Acao 4: salvar estado e encerrar programa.
    void acaoSuspenderJogoESair();

    // Acao 5: listar ranking Top 10.
    void acaoMostrarTop10() const;

    // Prepara estado de uma nova partida.
    void iniciarNovaPartida();

    // Executa o ciclo de turnos ate vitoria/empate/suspensao.
    void jogarPartida();

    // Mostra tabuleiro e dados da partida atual.
    void mostrarEstadoPartida() const;

    // Atualiza vitorias/empates/derrotas ao fim de cada partida.
    void atualizarEstatisticas(char vencedor);

    // Fabrica polimorfica usada ao carregar jogo salvo.
    std::unique_ptr<Jogador> criarJogadorPorTipo(const std::string& tipo, const std::string& nome, char simbolo) const;

    // Cria bot conforme dificuldade atual.
    std::unique_ptr<Jogador> criarBotAtual(char simbolo) const;

    // Persistencia do estado da partida.
    bool salvarPartida() const;

    // Carrega partida salva e reconstrui objetos polimorficos.
    bool carregarPartida();

    // Remove arquivo de save quando partida termina.
    void limparSave() const;

    // Persistencia do ranking.
    std::vector<RegistroRanking> carregarRanking() const;
    void salvarRanking(const std::vector<RegistroRanking>& ranking) const;
    void atualizarRanking(bool vitoria, bool empate, bool derrota);

    // Texto amigavel do modo atual.
    std::string dificuldadeTexto() const;

    // Solicita nome na primeira partida.
    void pedirNomeHumanoSeNecessario();

    // Estado do tabuleiro atual.
    Tabuleiro tabuleiro_;

    // Jogadores em ordem fixa: [0]=humano, [1]=computador.
    std::array<std::unique_ptr<Jogador>, 2> jogadores_;

    // Indice de turno atual (0 ou 1).
    int turnoAtual_;

    // Indica se existe partida ativa na memoria.
    bool partidaEmAndamento_;

    // Controla loop principal do programa.
    bool executando_;

    // Configuracoes selecionadas pelo usuario.
    Dificuldade dificuldadeAtual_;
    Inicio inicioAtual_;

    // Placar da sessao.
    Estatisticas estatisticas_;

    // Identidade do jogador humano.
    std::string nomeHumano_;

    // Caminhos dos arquivos de persistencia.
    std::string caminhoSave_;
    std::string caminhoTop10_;
};

#endif

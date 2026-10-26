#include "MotorJogo.h"

#include "JogadorComputador.h"
#include "JogadorHumano.h"

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>

// Inicializa estado padrao da aplicacao e caminhos de persistencia.
MotorJogo::MotorJogo()
    : turnoAtual_(0),
      partidaEmAndamento_(false),
      executando_(true),
      dificuldadeAtual_(Dificuldade::Elementar),
      inicioAtual_(Inicio::Aleatorio),
      estatisticas_{},
      nomeHumano_("Jogador"),
      caminhoSave_("data/savegame.txt"),
      caminhoTop10_("data/top10.txt") {
}

// Loop principal do menu.
void MotorJogo::executar() {
    while (executando_) {
        mostrarMenu();
        int opcao = lerOpcao(1, 6);

        switch (opcao) {
            case 1:
                acaoIniciarOuContinuar();
                break;
            case 2:
                acaoEscolherQuemInicia();
                break;
            case 3:
                acaoModoFuncionamento();
                break;
            case 4:
                acaoSuspenderJogoESair();
                break;
            case 5:
                acaoMostrarTop10();
                break;
            case 6:
                std::cout << "Tem certeza que deseja sair? (1=Sim, 2=Nao): ";
                if (lerOpcao(1, 2) == 1) {
                    executando_ = false;
                }
                break;
            default:
                break;
        }
    }
}

// Renderiza menu principal.
void MotorJogo::mostrarMenu() const {
    std::cout << "\n===== JOGO DA VELHA =====\n";
    std::cout << "1. Iniciar/continuar jogo\n";
    std::cout << "2. Escolher quem inicia\n";
    std::cout << "3. Modo de funcionamento\n";
    std::cout << "4. Suspender o jogo e sair\n";
    std::cout << "5. Mostrar top 10\n";
    std::cout << "6. Sair\n";
    std::cout << "Escolha: ";
}

// Le inteiro no intervalo [minimo, maximo] com recuperacao de erro.
int MotorJogo::lerOpcao(int minimo, int maximo) const {
    while (true) {
        int valor;
        if (!(std::cin >> valor)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada invalida. Digite um numero: ";
            continue;
        }

        if (valor < minimo || valor > maximo) {
            std::cout << "Opcao invalida. Tente novamente: ";
            continue;
        }

        return valor;
    }
}

// Opcao 1: continuar partida ativa/salva ou iniciar nova.
void MotorJogo::acaoIniciarOuContinuar() {
    if (partidaEmAndamento_) {
        jogarPartida();
        return;
    }

    std::ifstream testeSave(caminhoSave_);
    bool existeSave = testeSave.good();
    if (existeSave) {
        std::cout << "1. Novo jogo\n";
        std::cout << "2. Continuar jogo suspenso\n";
        std::cout << "Escolha: ";
        int escolha = lerOpcao(1, 2);

        if (escolha == 2) {
            if (carregarPartida()) {
                std::cout << "Jogo carregado com sucesso.\n";
                jogarPartida();
                return;
            }
            std::cout << "Falha ao carregar save. Iniciando novo jogo.\n";
        }
    }

    pedirNomeHumanoSeNecessario();
    iniciarNovaPartida();
    jogarPartida();
}

// Opcao 2: define quem comeca.
void MotorJogo::acaoEscolherQuemInicia() {
    std::cout << "\nQuem deve iniciar?\n";
    std::cout << "1. Humano\n";
    std::cout << "2. Computador\n";
    std::cout << "3. Aleatorio\n";
    std::cout << "Escolha: ";

    inicioAtual_ = static_cast<Inicio>(lerOpcao(1, 3));

    // Mudanca de configuracao invalida partida em curso.
    if (partidaEmAndamento_) {
        partidaEmAndamento_ = false;
        tabuleiro_.reset();
        limparSave();
        std::cout << "Partida em andamento cancelada devido a mudanca de configuracao.\n";
    }
}

// Opcao 3: define dificuldade do bot.
void MotorJogo::acaoModoFuncionamento() {
    std::cout << "\nDificuldade:\n";
    std::cout << "1. Elementar (aleatorio)\n";
    std::cout << "2. Basico (greedy)\n";
    std::cout << "3. Medio (ganha ou bloqueia)\n";
    std::cout << "Escolha: ";

    dificuldadeAtual_ = static_cast<Dificuldade>(lerOpcao(1, 3));

    // Enunciado pede cancelamento de jogo ativo ao mudar modo.
    if (partidaEmAndamento_) {
        partidaEmAndamento_ = false;
        tabuleiro_.reset();
        limparSave();
        std::cout << "Partida em andamento cancelada devido a mudanca de modo.\n";
    }
}

// Opcao 4: salva e encerra programa.
void MotorJogo::acaoSuspenderJogoESair() {
    if (!partidaEmAndamento_) {
        std::cout << "Nao ha partida ativa para suspender.\n";
        return;
    }

    if (salvarPartida()) {
        std::cout << "Jogo suspenso e salvo em arquivo. Encerrando programa.\n";
        executando_ = false;
        partidaEmAndamento_ = false;
    } else {
        std::cout << "Falha ao salvar o jogo.\n";
    }
}

// Opcao 5: exibe ranking Top 10.
void MotorJogo::acaoMostrarTop10() const {
    std::vector<RegistroRanking> ranking = carregarRanking();

    std::cout << "\n===== TOP 10 =====\n";
    if (ranking.empty()) {
        std::cout << "Ainda nao ha registros.\n";
        return;
    }

    std::cout << "Pos Nome                 V  E  D  Pontos\n";
    int limite = static_cast<int>(std::min<std::size_t>(10, ranking.size()));
    for (int i = 0; i < limite; ++i) {
        const RegistroRanking& r = ranking[static_cast<std::size_t>(i)];
        std::cout << (i + 1) << "   " << r.nome;
        if (r.nome.size() < 20) {
            std::cout << std::string(20 - r.nome.size(), ' ');
        }
        std::cout << " " << r.vitorias << "  " << r.empates << "  " << r.derrotas << "  " << r.pontos() << "\n";
    }
}

// Cria jogadores e prepara estado inicial da partida.
void MotorJogo::iniciarNovaPartida() {
    jogadores_[0] = std::make_unique<JogadorHumano>(nomeHumano_, 'X');
    jogadores_[1] = criarBotAtual('O');

    tabuleiro_.reset();
    partidaEmAndamento_ = true;

    if (inicioAtual_ == Inicio::Humano) {
        turnoAtual_ = 0;
    } else if (inicioAtual_ == Inicio::Computador) {
        turnoAtual_ = 1;
    } else {
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(0, 1);
        turnoAtual_ = dist(rng);
    }
}

// Loop de turno ate condicao terminal (vitoria/empate/suspensao).
void MotorJogo::jogarPartida() {
    while (partidaEmAndamento_) {
        mostrarEstadoPartida();

        Jogador& atual = *jogadores_[static_cast<std::size_t>(turnoAtual_)];
        int jogada = atual.escolherJogada(tabuleiro_);

        // Somente humano pode solicitar suspensao via 0.
        if (jogada == 0 && atual.tipo() == "humano") {
            if (salvarPartida()) {
                std::cout << "Partida suspensa com sucesso.\n";
                partidaEmAndamento_ = false;
            } else {
                std::cout << "Falha ao suspender partida.\n";
            }
            return;
        }

        if (!tabuleiro_.aplicarJogada(jogada, atual.simbolo())) {
            std::cout << "Jogada invalida. Tente novamente.\n";
            continue;
        }

        char vencedor = tabuleiro_.verificarVencedor();
        if (vencedor != ' ') {
            tabuleiro_.imprimirTabuleiro();
            std::cout << "\nVencedor: " << atual.nome() << " (" << atual.simbolo() << ")\n";
            atualizarEstatisticas(vencedor);
            partidaEmAndamento_ = false;
            limparSave();
            return;
        }

        if (tabuleiro_.isCheio()) {
            tabuleiro_.imprimirTabuleiro();
            std::cout << "\nEmpate!\n";
            atualizarEstatisticas(' ');
            partidaEmAndamento_ = false;
            limparSave();
            return;
        }

        turnoAtual_ = 1 - turnoAtual_;
    }
}

// Mostra painel resumido da partida atual.
void MotorJogo::mostrarEstadoPartida() const {
    std::cout << "\n===== PARTIDA =====\n";
    std::cout << "Modo: " << dificuldadeTexto() << "\n";
    std::cout << "Jogada numero: " << (tabuleiro_.jogadasRealizadas() + 1) << "\n";
    std::cout << "Turno: " << jogadores_[static_cast<std::size_t>(turnoAtual_)]->nome()
              << " (" << jogadores_[static_cast<std::size_t>(turnoAtual_)]->simbolo() << ")\n";
    std::cout << "Estatisticas (V/E/D): " << estatisticas_.vitorias << "/"
              << estatisticas_.empates << "/" << estatisticas_.derrotas << "\n";

    tabuleiro_.imprimirTabuleiro();
}

// Atualiza placar local e ranking persistente.
void MotorJogo::atualizarEstatisticas(char vencedor) {
    char simboloHumano = jogadores_[0]->simbolo();
    if (vencedor == ' ') {
        ++estatisticas_.empates;
        atualizarRanking(false, true, false);
    } else if (vencedor == simboloHumano) {
        ++estatisticas_.vitorias;
        atualizarRanking(true, false, false);
    } else {
        ++estatisticas_.derrotas;
        atualizarRanking(false, false, true);
    }
}

// Reconstrucao polimorfica para partidas carregadas.
std::unique_ptr<Jogador> MotorJogo::criarJogadorPorTipo(const std::string& tipo, const std::string& nome, char simbolo) const {
    if (tipo == "humano") {
        return std::make_unique<JogadorHumano>(nome, simbolo);
    }
    if (tipo == "bot_aleatorio") {
        return std::make_unique<BotAleatorio>(nome, simbolo);
    }
    if (tipo == "bot_basico") {
        return std::make_unique<BotBasico>(nome, simbolo);
    }
    if (tipo == "bot_medio") {
        return std::make_unique<BotMedio>(nome, simbolo);
    }
    return nullptr;
}

// Fabrica do bot ativo conforme configuracao de dificuldade.
std::unique_ptr<Jogador> MotorJogo::criarBotAtual(char simbolo) const {
    if (dificuldadeAtual_ == Dificuldade::Elementar) {
        return std::make_unique<BotAleatorio>("Computador", simbolo);
    }
    if (dificuldadeAtual_ == Dificuldade::Basico) {
        return std::make_unique<BotBasico>("Computador", simbolo);
    }
    return std::make_unique<BotMedio>("Computador", simbolo);
}

// Grava snapshot completo da partida atual.
bool MotorJogo::salvarPartida() const {
    std::ofstream out(caminhoSave_);
    if (!out.is_open()) {
        return false;
    }

    out << "NOME=" << nomeHumano_ << "\n";
    out << "DIFICULDADE=" << static_cast<int>(dificuldadeAtual_) << "\n";
    out << "INICIO=" << static_cast<int>(inicioAtual_) << "\n";
    out << "TURNO=" << turnoAtual_ << "\n";
    out << "PARTIDA=" << (partidaEmAndamento_ ? 1 : 0) << "\n";
    out << "ESTATS=" << estatisticas_.vitorias << "," << estatisticas_.empates << "," << estatisticas_.derrotas << "\n";
    out << "TAB=" << tabuleiro_.serializar() << "\n";

    for (int i = 0; i < 2; ++i) {
        const Jogador& j = *jogadores_[static_cast<std::size_t>(i)];
        out << "J" << i << "=" << j.tipo() << "," << j.nome() << "," << j.simbolo() << "\n";
    }

    return true;
}

// Le save, valida e restaura estado de execucao.
bool MotorJogo::carregarPartida() {
    std::ifstream in(caminhoSave_);
    if (!in.is_open()) {
        return false;
    }

    std::string nome;
    int dificuldade = 1;
    int inicio = 3;
    int turno = 0;
    int partida = 0;
    Estatisticas est;
    std::string tabEstado;
    std::string jTipo[2];
    std::string jNome[2];
    char jSimbolo[2] = {'X', 'O'};

    std::string linha;
    while (std::getline(in, linha)) {
        std::size_t eq = linha.find('=');
        if (eq == std::string::npos) {
            continue;
        }

        std::string chave = linha.substr(0, eq);
        std::string valor = linha.substr(eq + 1);

        if (chave == "NOME") {
            nome = valor;
        } else if (chave == "DIFICULDADE") {
            dificuldade = std::stoi(valor);
        } else if (chave == "INICIO") {
            inicio = std::stoi(valor);
        } else if (chave == "TURNO") {
            turno = std::stoi(valor);
        } else if (chave == "PARTIDA") {
            partida = std::stoi(valor);
        } else if (chave == "ESTATS") {
            std::replace(valor.begin(), valor.end(), ',', ' ');
            std::istringstream iss(valor);
            iss >> est.vitorias >> est.empates >> est.derrotas;
        } else if (chave == "TAB") {
            tabEstado = valor;
        } else if (chave == "J0" || chave == "J1") {
            int idx = (chave == "J0") ? 0 : 1;
            std::replace(valor.begin(), valor.end(), ',', '\n');
            std::istringstream iss(valor);
            std::getline(iss, jTipo[idx]);
            std::getline(iss, jNome[idx]);
            std::string simboloStr;
            std::getline(iss, simboloStr);
            if (!simboloStr.empty()) {
                jSimbolo[idx] = simboloStr[0];
            }
        }
    }

    auto p0 = criarJogadorPorTipo(jTipo[0], jNome[0], jSimbolo[0]);
    auto p1 = criarJogadorPorTipo(jTipo[1], jNome[1], jSimbolo[1]);
    if (!p0 || !p1) {
        return false;
    }

    if (!tabuleiro_.desserializar(tabEstado)) {
        return false;
    }

    jogadores_[0] = std::move(p0);
    jogadores_[1] = std::move(p1);
    nomeHumano_ = nome;
    dificuldadeAtual_ = static_cast<Dificuldade>(dificuldade);
    inicioAtual_ = static_cast<Inicio>(inicio);
    turnoAtual_ = turno;
    partidaEmAndamento_ = (partida == 1);
    estatisticas_ = est;
    return true;
}

// Exclui arquivo de save; usado apos fim de partida.
void MotorJogo::limparSave() const {
    std::remove(caminhoSave_.c_str());
}

// Carrega ranking persistente do disco.
std::vector<RegistroRanking> MotorJogo::carregarRanking() const {
    std::vector<RegistroRanking> ranking;

    std::ifstream in(caminhoTop10_);
    if (!in.is_open()) {
        return ranking;
    }

    std::string linha;
    while (std::getline(in, linha)) {
        if (linha.empty()) {
            continue;
        }

        std::replace(linha.begin(), linha.end(), ';', ' ');
        std::istringstream iss(linha);

        RegistroRanking r;
        iss >> r.nome >> r.vitorias >> r.empates >> r.derrotas;
        if (!r.nome.empty()) {
            ranking.push_back(r);
        }
    }

    return ranking;
}

// Persiste no maximo 10 registros ja ordenados.
void MotorJogo::salvarRanking(const std::vector<RegistroRanking>& ranking) const {
    std::ofstream out(caminhoTop10_);
    if (!out.is_open()) {
        return;
    }

    int limite = static_cast<int>(std::min<std::size_t>(10, ranking.size()));
    for (int i = 0; i < limite; ++i) {
        const RegistroRanking& r = ranking[static_cast<std::size_t>(i)];
        out << r.nome << ';' << r.vitorias << ';' << r.empates << ';' << r.derrotas << "\n";
    }
}

// Atualiza/insere jogador no ranking e reordena por desempenho.
void MotorJogo::atualizarRanking(bool vitoria, bool empate, bool derrota) {
    std::vector<RegistroRanking> ranking = carregarRanking();

    auto it = std::find_if(ranking.begin(), ranking.end(), [this](const RegistroRanking& r) {
        return r.nome == nomeHumano_;
    });

    if (it == ranking.end()) {
        RegistroRanking novo;
        novo.nome = nomeHumano_;
        ranking.push_back(novo);
        it = std::prev(ranking.end());
    }

    if (vitoria) {
        ++it->vitorias;
    }
    if (empate) {
        ++it->empates;
    }
    if (derrota) {
        ++it->derrotas;
    }

    std::sort(ranking.begin(), ranking.end(), [](const RegistroRanking& a, const RegistroRanking& b) {
        if (a.pontos() != b.pontos()) {
            return a.pontos() > b.pontos();
        }
        if (a.vitorias != b.vitorias) {
            return a.vitorias > b.vitorias;
        }
        return a.nome < b.nome;
    });

    salvarRanking(ranking);
}

// Retorna label textual da dificuldade atual.
std::string MotorJogo::dificuldadeTexto() const {
    if (dificuldadeAtual_ == Dificuldade::Elementar) {
        return "Elementar";
    }
    if (dificuldadeAtual_ == Dificuldade::Basico) {
        return "Basico";
    }
    return "Medio";
}

// Solicita nome uma unica vez no inicio da sessao.
void MotorJogo::pedirNomeHumanoSeNecessario() {
    if (nomeHumano_ != "Jogador") {
        return;
    }

    std::cout << "Digite o nome do jogador (sem espacos): ";
    std::cin >> nomeHumano_;
    if (nomeHumano_.empty()) {
        nomeHumano_ = "Jogador";
    }
}

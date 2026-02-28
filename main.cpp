// Fazendo o menu básico do jogo da velha
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void displayMenu() {
    cout << "Bem-vindo ao Jogo da Velha!" << endl;
    cout << "1. Jogar" << endl;
    cout << "2. Sair" << endl;
    cout << "Escolha uma opção: ";
}

int main() {
    int choice;
    
    // Ele irá ficar no loop ate que o usuario escolha a opção de sair 
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Iniciando o jogo..." << endl;
                // Aqui você pode chamar a função para iniciar o jogo
                break;
            case 2:
                cout << "Saindo do jogo. Até mais!" << endl;
                break;
            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }
    } while (choice != 2);

    return 0;
}
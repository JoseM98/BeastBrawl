#include <iostream>
#include <memory>
#include "Game.h"

#include "CLPhysics/CLPhysics.h"

using namespace std;

int main() {
    //CLPhysics::RunTests();
    try {
        Game* game = Game::GetInstance();

        game->InitGame();
        game->SetState(State::States::MENU);
        game->MainLoop();
        game->TerminateGame();
    } catch (...) {
        cout << "Algo ha fallado" << endl;
    }

    return 0;
}
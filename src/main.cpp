#include "Game.h"
#include "lib/glm/vec3.hpp"
#include <iostream>

using namespace std;


#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"

int main()
{

    Game *game = Game::GetInstance();

    
    game->SetState(State::States::INGAME);
    game->InitGame();
	
	return 0;
}
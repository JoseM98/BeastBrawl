#include "Game.h"
#include "lib/glm/vec3.hpp"
#include <iostream>

using namespace std;

int main()
{

    Game *game = Game::GetInstance();

    
        game->SetState(State::States::INGAME);
        game->InitGame();
    }
    catch(...)
    {
        std::cout << "Algo ha fallado" << std::endl;
    }
    
    
	std::cout <<"SALDLASDASD\n";
	return 0;
}
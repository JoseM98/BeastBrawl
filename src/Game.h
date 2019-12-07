#pragma once

#include <iostream>
#include "State/State.h"
#include "State/StateMenu.h"
#include "State/StateInGame.h"
#include "Facade/Render/RenderFacadeManager.h"
#include "Facade/Sound/SoundFacadeManager.h"



class Game{
    public:
        static Game* GetInstance();
        void SetState(State::States stateType);
        State* GetState() { return currentState; };
        State* GetLastState() { return lastState; };
        void InitGame();
        



    private:
        static Game *game;
        //State::States ActualState;
        State *currentState;
        State *lastState;
        RenderFacadeManager* renderFacadeManager;

        Game();
        virtual ~Game();
        void MainLoop();
};


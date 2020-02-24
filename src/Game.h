#pragma once

#include "State/State.h"
#include <Aliases.h>

#include <iostream>
#include <memory>
#include <chrono>
#include "Constants.h"

using namespace std;
using namespace std::chrono;

class State;

class Game{
    public:
        virtual ~Game(){};
        static Game* GetInstance();
        void SetState(State::States stateType);
        shared_ptr<State> GetState() { return currentState; };
        shared_ptr<State> GetLastState() { return lastState; };
        void InitGame();
        void MainLoop();
        void TerminateGame();


        //EventManager
        void SetStateMenu(DataMap* d);
        void SetStatePause(DataMap* d);
        void SetStateInGameSingle(DataMap* d);
        void SetStateInGameMulti(DataMap* d);
        void SetStateEndRace(DataMap* d);
        void SetStateLobbyMulti(DataMap* d);
        
    



    private:
        Game(){};
        static Game* game;
        void SuscribeEvents();

        shared_ptr<State> currentState;
        shared_ptr<State> gameState;
        shared_ptr<State> lastState;

        bool gameStarted = false;


        time_point<system_clock> start;
        int64_t timeElapsed { 0 };
        bool gameMarkedToDelete {false};
};


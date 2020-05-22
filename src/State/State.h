#pragma once


#include <iostream>
using namespace std;

class State{
    public:
        enum States{ NO_STATE, INTRO, MENU, SELECT_CHARACTER, GAME_OPTIONS, TOURNAMENT_OPTIONS, INGAME_SINGLE, INGAME_MULTI, ENDRACE, ENDTOURNAMENT, 
                    PAUSE, CONTROLS, CREDITS, LOBBY_MULTI, SETTINGS};
        State();
        virtual ~State() = 0;
        virtual void InitState() = 0;
        virtual void Update() = 0;
        virtual void Input() = 0;
        virtual void Render() = 0;
        virtual States GetState() = 0;
        virtual States GetEstado() { return States::NO_STATE; };


    private:

};

#pragma once


#include <iostream>


class State{
    public:
        enum States{ NO_STATE, INTRO, MENU, MAP, INGAME, ENDRACE, PAUSE, CONTROLS, CREDITS};
        State();
        virtual ~State();

        virtual void InitState() = 0;
        virtual void Update() = 0;
        virtual void Render() = 0;
        virtual States GetEstado() { return States::NO_STATE; };


    private:

};

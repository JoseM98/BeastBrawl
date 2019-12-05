#pragma once

#include <iostream>
#include "InputFacade.h"
#include "../../../lib/sdl/SDL.h"


using namespace std;

class InputFacadeClover : public InputFacade{
    public:
        InputFacadeClover();
        ~InputFacadeClover();
        void CheckInputs(float, Entity*, Entity*) override;
};
#pragma once

#include <iostream>
#include "InputFacade.h"
#include "InputFacadeIrrlicht.h"
#include "InputFacadeClover.h"

class InputFacadeManager{

public:
    ~InputFacadeManager(){ delete facade;};

    void InitializeIrrlicht() {
        facade = new InputFacadeIrrlicht();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
        facade = new InputFacadeClover();
    }

    InputFacade *GetInputFacade() { return facade; };
    static InputFacadeManager* GetInstance();

private:
    InputFacadeManager() : facade(nullptr){}
    InputFacade *facade;
    static InputFacadeManager* instance;
};

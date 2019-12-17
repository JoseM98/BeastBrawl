#pragma once

#include <iostream>
#include <memory>
#include "PhysicsFacade.h"
#include "PhysicsFacadeIrrlicht.h"

using namespace std;
class PhysicsFacadeManager{

public:
    ~PhysicsFacadeManager(){delete facade;};

    void InitializeIrrlicht() {
        facade = new PhysicsFacadeIrrlicht();
    }

    void InitializeClover() {
        // TODO: crear motor de clover
    }

    PhysicsFacade* GetPhysicsFacade() { return facade; };
    static PhysicsFacadeManager* GetInstance();

private:
    PhysicsFacadeManager() : facade(nullptr){};
    PhysicsFacade* facade;
    static PhysicsFacadeManager* instance;
};

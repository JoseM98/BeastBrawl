#pragma once

#include "State.h"

#include <EventManager/EventManager.h>
#include <Facade/Input/InputFacadeManager.h>
#include <Facade/Physics/PhysicsFacadeManager.h>
#include <Facade/Render/RenderFacadeManager.h>
#include <Facade/Sound/SoundFacadeManager.h>



class StateMenu : public State{
    public:
        StateMenu();
        ~StateMenu() override = default;
        void Input() override;
        void InitState() override;
        void Update() override;
        void Render() override;
        States GetState() override { return State::States::MENU; };


    private:
        RenderFacade* renderEngine = { nullptr };
        //InputFacade* inputEngine = { nullptr };
        //PhysicsFacade* physicsEngine = { nullptr };
        SoundFacade* soundEngine = { nullptr };


	
};


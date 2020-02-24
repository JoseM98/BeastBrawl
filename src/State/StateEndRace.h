#pragma once

#include "../EventManager/EventManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Sound/SoundFacadeManager.h"
#include "../Facade/Sound/SoundFacade.h"
//#include "../Game.h"
#include "../State/State.h"


class StateEndRace : public State{
    public:
        StateEndRace();
        ~StateEndRace() = default;
        void Input() override;
        void InitState() override;
        void Update() override;
        void Render(double timeElapsed) override;
        States GetState() { return State::States::MENU; };


    private:
        RenderFacade* renderEngine = { nullptr };
        SoundFacade* soundEngine = { nullptr };
        //InputFacade* inputEngine = { nullptr };
        //PhysicsFacade* physicsEngine = { nullptr };


	
};


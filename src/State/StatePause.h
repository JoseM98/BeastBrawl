#pragma once

#include "../EventManager/EventManager.h"
#include "../Facade/Input/InputFacadeManager.h"
#include "../Facade/Physics/PhysicsFacadeManager.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Facade/Sound/SoundFacadeManager.h"
#include "../Facade/Sound/SoundFacade.h"
//#include "../Game.h"
#include "../State/State.h"


class StatePause : public State{
    public:
        StatePause();
        ~StatePause() = default;
        void Input() override;
        void InitState() override;
        void Update() override;
        void Render() override;
        States GetState() { return State::States::PAUSE; };


    private:
        RenderFacade* renderEngine = { nullptr };
        //InputFacade* inputEngine = { nullptr };
        //PhysicsFacade* physicsEngine = { nullptr };
        SoundFacade* soundEngine = { nullptr };


	
};


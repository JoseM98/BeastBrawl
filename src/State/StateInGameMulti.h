#pragma once

#include "StateInGame.h"
#include <chrono>

using namespace std::chrono;

class SystemOnline;

class StateInGameMulti : public StateInGame {
   public:
    StateInGameMulti();
    StateInGameMulti(uint16_t IdOnline, const vector<uint16_t> IdPlayersOnline);
    ~StateInGameMulti();
    void InitState() override;
    void Input() override;
    void Update() override;
    void Render(double timeElapsed) override;
    States GetState() { return State::States::INGAME_MULTI; };

   private:
    void InitializeCLPhysics(ManCar &manCars, ManBoundingWall &, ManBoundingOBB &, ManBoundingGround &, ManPowerUp &, ManNavMesh&, ManBoxPowerUp&, ManTotem &) override;
    void InitializeManagers(Physics *physics, Camera *cam) override;
    void InitializeSystems(ManCar &manCars, ManBoundingWall &, ManBoundingOBB &, ManBoundingGround &, ManPowerUp &, ManNavMesh&, ManBoxPowerUp&, ManTotem &) override;
    void InitializeFacades() override;
    void AddElementsToRender() override;

    void CAMBIARCosasDeTotemUpdate() override;

    unique_ptr<SystemOnline> sysOnline;

    time_point<system_clock> lastTimeSentInputs;
    time_point<system_clock> lastTimeSentSync;
};

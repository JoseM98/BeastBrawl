#pragma once

#include "StateInGame.h"
#define FRAME_RATE 1.0

class StateInGameSingle : public StateInGame {
   public:
    StateInGameSingle();
    ~StateInGameSingle();
    void InitState() override;
    void Input() override;
    void Update() override;
    void Render() override;
    States GetState() { return State::States::INGAME_SINGLE; };

   private:
    void InitializeCLPhysics(ManCar &manCars, ManBoundingWall &ManBoundingWall) override;
    void InitializeManagers(Physics *physics, Camera *cam) override;
    void InitializeSystems(ManCar &manCars, ManBoundingWall &manBoundingWall) override;
    void InitializeFacades() override;
    void AddElementsToRender() override;

    void CAMBIARInicializarCarAIS(ManCar &, ManWayPoint &);
    void CAMBIARCosasNavMesh(ManCar &, ManNavMesh &);
    void CAMBIARCosasDeTotemUpdate() override;

    time_point<system_clock> start;
    double updateTickTime = 1000000.0/FRAME_RATE;  // mircroseconds 16000  -> 16,000 milliseconds
    double percentTick = 0.0;
    double timeElapsed = 0.0;
};

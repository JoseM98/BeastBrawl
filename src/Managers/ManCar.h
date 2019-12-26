#pragma once

#include <stdlib.h> /* srand, rand */
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "../../lib/glm/vec3.hpp"
#include "../Aliases.h"
#include "../Components/CWayPoint.h"
#include "../Components/CTransformable.h"
#include "../Components/CSpeed.h"
#include "Manager.h"

using namespace std;
class Car;
class CarAI;
class Data;
class Physics;
class Camera;

class ManCar : public Manager {
   public:
    ManCar();
    ManCar(Physics *, Camera *);
    ~ManCar();

    void CreateCar();
    void CreateMainCar();
    void UpdateCar();
    void UpdateCarAI(CarAI* car);
    shared_ptr<Car>& GetCar() { return car; };

    void CreateCarAI();
    void CreateCarAI(glm::vec3 _position, CWayPoint* _waypoint);
    vector<shared_ptr<CarAI>> GetEntitiesAI() const { return CarAIs; };
    CTransformable* calculateCloserCar(Entity* actualCar);
    bool carInVisionRange(Entity* actualCar, Entity* otherCar, uint32_t rangeVision);
    bool anyCarInVisionRange(Entity* actualCar, uint32_t rangeVision);
    void Integrate(float) override;

   private:
    Physics *physics;
    Camera *cam;
    void AccelerateCar(DataMap d);
    void SubscribeToEvents() override;
    void TurnLeftCar(DataMap d);
    void TurnRightCar(DataMap d);
    void NotAcceleratingOrDecelerating(DataMap d);
    void Decelerate(DataMap d);
    void NotTurning(DataMap d);
    void CollisionPowerUp(DataMap d);
    void CollisionPowerUpAI(DataMap d);
    void CatchTotemPlayer(DataMap d);
    void CatchTotemAI(DataMap d);
    void UseTotem(Entity* carWinTotem);
    void ThrowTotem(Entity* carLoseTotem);
    bool useRoboJorobo(Entity* newCarWithTotem);

    void ThrowPowerUp(DataMap d);
    void ThrowPowerUpAI(DataMap d);
    void CatchPowerUp(DataMap d);
    void CatchPowerUpAI(DataMap d);
    shared_ptr<Car> car;
    vector<shared_ptr<CarAI>> CarAIs;
};

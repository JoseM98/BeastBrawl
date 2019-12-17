#pragma once

#include <iostream>
#include <memory>
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Managers/ManCar.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManWayPoint.h"

class Blackboard{


    public:
    Blackboard(CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint);
    ~Blackboard() {};

    CarAI* actualCar;
    ManCar* manCars;
    ManPowerUp* manPowerUps;
    ManBoxPowerUp* manBoxPowerUps;
    ManTotem* manTotems;
    ManWayPoint* manWayPoint;
};


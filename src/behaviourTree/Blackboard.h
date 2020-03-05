#pragma once

#include <Entities/CarHuman.h>
#include <Entities/CarAI.h>
#include <Managers/ManCar.h>
#include <Managers/ManPowerUp.h>
#include <Managers/ManBoxPowerUp.h>
#include <Managers/ManTotem.h>
#include <Managers/ManWayPoint.h>
#include <Managers/ManBoundingWall.h>
#include <Managers/ManNavMesh.h>
#include <Systems/SystemFuzzyLogicAI.h>
#include <Systems/SteeringBehaviours.h>

class Blackboard {
    
    public:

        Blackboard( CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint,
                    ManNavMesh* m_manNavMesh);

        Blackboard( CarAI* m_actualCar, ManCar* m_manCars, ManPowerUp* m_manPowerUps, ManBoxPowerUp* m_manBoxPowerUps, ManTotem* m_manTotems, ManWayPoint* m_manWayPoint,
                    SystemFuzzyLogicAI* m_systemFuzzyLogicAI, SteeringBehaviours* m_steeringBehaviours, ManNavMesh* m_manNavMesh, ManBoundingWall* m_manBoundingWall);

        //TODO: Si todo esto son *, ¿no se deberia llamar al destructor y que este haga delete de todos ellos?
        ~Blackboard() = default;

        CarAI* actualCar {nullptr};
        ManCar* manCars {nullptr};
        ManPowerUp* manPowerUps {nullptr};
        ManBoxPowerUp* manBoxPowerUps {nullptr};
        ManTotem* manTotems {nullptr};
        ManWayPoint* manWayPoint {nullptr};
        ManNavMesh* manNavMesh {nullptr};
        ManBoundingWall* manBoundingWall {nullptr};
        SystemFuzzyLogicAI* systemFuzzyLogicAI {nullptr};
        SteeringBehaviours* steeringBehaviours {nullptr};
};


#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include "Manager.h"
#include "ManCar.h"
#include "../../include/glm/vec3.hpp"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"


class NavMesh;


using namespace std;
struct Car;
struct CarAI;
struct Data;

class ManNavMesh : public Manager{
   public:
    ManNavMesh();
    ~ManNavMesh(){};

    void CreateNavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, vector<int> waypoints);
    void CreateNavMesh();

    void UpdateNavMeshPlayer(Entity* carPlayer);
    //void UpdateNavMeshCarsAI(ManCar* manCars);

    // vector<shared_ptr<NavMesh>> GetEntities() const {
    //     return navMeshes;
    // };

    // vector<shared_ptr<NavMesh>> GetEntities()  {
    //     return navMeshes;
    // };


   private:
    //vector<shared_ptr<NavMesh>> navMeshes;
    void SubscribeToEvents() override;
    void ActualizeNavMeshTotem(DataMap d);
    void ActualizeNavMeshCarAI(DataMap d);
    //void ActualizeNavMeshCarPlayer(Car* carPlayer);
};
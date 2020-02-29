#pragma once

#include "ManCar.h"

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <Facade/Render/RenderFacadeManager.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>


class NavMesh;
struct ManTotem;

using namespace std;
struct Car;
struct CarAI;
struct Data;
struct CTransformable;

class ManNavMesh : public Manager{
   public:
    //ManNavMesh(){};
    ManNavMesh();
    ~ManNavMesh(){};

    void CreateNavMesh(glm::vec3 pos, glm::vec3 rot, float width, float height, float depth, vector<int> waypoints);
    void CreateNavMesh();

    void UpdateNavMeshEntity(Entity* entity_);

    //static ManNavMesh& getInstance() {
    //    static ManNavMesh instance;
    //    return instance;
    //};

    //void UpdateNavMeshCarsAI(ManCar* manCars);

    // vector<shared_ptr<NavMesh>> GetEntities() const {
    //     return navMeshes;
    // };

    // vector<shared_ptr<NavMesh>> GetEntities()  {
    //     return navMeshes;
    // };
    int CalculateNavMesh(glm::vec3 &) const;    // version para llamar desde fuera de la clase NavMesh

   private:
    //vector<shared_ptr<NavMesh>> navMeshes;
    void SubscribeToEvents() override;
    void ActualizeNavMeshTotem(DataMap* d);
    void InitNavMeshTotem(ManTotem *manTotems);
    void ActualizeNavMeshCarAI(DataMap* d);
    int CalculateNavMesh(CTransformable &) const;
    //void ActualizeNavMeshCarPlayer(Car* carPlayer);
};
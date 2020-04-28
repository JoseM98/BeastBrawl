#pragma once

#include "Component.h"

#include <glm/vec3.hpp>
#include <string>
#include <stack>
#include <vector>
#include "../Entities/Car.h"
#include "../Entities/Totem.h"
#include "../Entities/BoxPowerUp.h"



class CBrainAI : public Component{
public:
    CBrainAI();
    ~CBrainAI(){};

    void CleanVisionRange();



    // CTargetNavMesh
    int targetNavMesh {-1};  //NavMesh actual del coche/entidad     //ponemos -1 por defecto ya que haremos el calculo al empezar la partida


    // CPath
    std::stack<int> stackPath;


    // CMovementType
    std::string movementType {"Empty"};     // Para el debug de la IA decirte que decision estas tomando


    // CPosDestinatation
    // glm::vec3 pos_Destinatation = glm::vec3(0.0,0.0,0.0);
    // int id_Destinatation;
    // float radious_Destinatation = 30.0f;
    //int type;
    // tenemos varios tipos:
    // 0 : cuando el destino es un nodo
    // 1 : cuando el destino es un Totem o un coche 


    // CWayPoint
    // glm::vec3 position_WayPoint = glm::vec3(0.0,0.0,0.0);
    // int type_WayPoint;
    // int id_WayPoint;
    // float radious_WayPoint = 30.0f;


    // elementos en el rango de vision
    int fov = 70;
    float distanceVision {600};
    std::vector<Entity*> carInVision;
    std::vector<Entity*> boxInVision;
    Entity* totemInVision {nullptr};
    
    // objetivo
    Car* targetCar {nullptr};

    // Pensamiento de la IA = mirar minimapa, mirar marcadores, etc, durante ese tiempo no actua
    bool thinking {false};

};
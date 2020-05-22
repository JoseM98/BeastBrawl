#pragma once

#include "Component.h"
#include "Components/CBoundingSphere.h"
#include "Components/CId.h"

#include <iostream>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class CBoundingRay : public Component {
   public:
    CBoundingRay();
    ~CBoundingRay(){};
    

    // para evitar atraparse en la esquina de los planos
    const float baseDistancePlane = 10.0;                           // lo largo base que es el rayo
    const float baseDistanceOBB = 30.0;                           // lo largo base que es el rayo
    uint32_t iteratorSamePlane;                                     // iterador para cambiar del plano de colision
    const uint32_t maxItSamePlane = 30 /* * deltaTime */;           // ciclos que tarda en cambiar de plano
    const uint32_t maxItDistinctPlane = 180 /* * deltaTime */;      // ciclos que tarda en cambiar de plano
    CId* previousIdCollided;
    CId* prevToPrevIdCollided;
    glm::vec3 target;                                               // punto al que tiene que dirigirse, resultado de la normal del plano


    // para evitar vibracion en la evasion de un objeto (coche)
    const float baseDistanceSphere = 50.0;                          // lo largo base que es el rayo
    uint32_t iteratorSameSphere;
    const uint32_t maxItSameSphere = 30;                            // ciclos que tarda en cambiar de esfera
    CBoundingSphere* previousSphere;
    glm::vec2 vectorForce;

};
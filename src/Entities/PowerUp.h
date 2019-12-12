#pragma once

#include "Entity.h"
#include "../../lib/glm/vec3.hpp"
#include "../Components/CPowerUp.h"
#include <iostream>
#include <map>

using namespace std;


class PowerUp : public Entity{

public:
    PowerUp();
    //PowerUp(glm::vec3 _position);
    PowerUp(glm::vec3 _position, glm::vec3 _rotation, typeCPowerUp typePowerUp);
    PowerUp(glm::vec3 _position, glm::vec3 _rotation, typeCPowerUp typePowerUp, string texture, string mesh);
    ~PowerUp();

};

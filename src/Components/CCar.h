#pragma once

#include "Component.h"

#include <iostream>
#include <string>
#include <Constants.h>
#include "../../include/glm/vec3.hpp"

using namespace std;
using namespace glm;

class CCar : public Component {
   public:
    CCar();
    CCar(float maxSpeed, float acceleration, float carFriction, float carSlowDown);
    ~CCar(){};

    friend ostream &operator<<(ostream &out, CCar &c) {
        //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
        return out;
    }

    float speed = 0.0;
    float maxSpeed;
    float acceleration = 0.0;
    float reverseMaxSpeed = -60.0;
    float wheelRotation = 0.0;
    const float incrementWheelRotation = 75;  // velocidad por iteracion que se gira la rueda
    const float decrementWheelRotation = 75;  // velocidad por iteracion que se recupera la rueda
    float maxWheelRotation = 10;
    float friction;
    float slowDown;
};
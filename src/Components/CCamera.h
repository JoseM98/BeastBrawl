#pragma once

#include "Component.h"
#include "../../include/glm/glm.hpp"

#include <string>
#include <iostream>
#include <glm/vec3.hpp>
#include "../Constants.h"

using namespace std;
using namespace glm;
enum CamType{
    NORMAL_CAM,
    INVERTED_CAM,
    TOTEM_CAM
};
class CCamera : public Component
{
    public:
        CCamera();
        CCamera(float _tarx, float _tary, float _tarz, float _rotExtraY);
        ~CCamera(){};
    
    
        void ApplyPhysics(float maxSpeed_, float acc_, float revMaxSpeed_, float slowDown_);
    
        friend ostream &operator<<(ostream &out, CCamera &c)
        {
            //out << "(" << c.GetX() << "," << c.GetY() << "," << c.GetZ() << ")";
            return out;
        }
    
        float tarX;
        float tarY;
        float tarZ;
    
        float rotExtraY;
        float incrementrotExtraY = 0.63;  // velocidad por iteracion que se gira la camara comprovar que es relativo al giro de la rueda
        float decrementrotExtraY = 0.95;  // velocidad por iteracion que se recupera la camara comprovar que es relativo al giro de la rueda
        float rotExtraCamera = 8.0;   // grados extra de la camara respecto al coche 
        CamType camType = CamType::NORMAL_CAM;        // Tipo de seguimiento

        float speed {0.0};
        float slowDown;
        float maxSpeed;
        float acceleration;
        float reverseMaxSpeed;

        glm::vec3 target;


        // cosas jose
        const float perfectDistance {34.0};
        const float minDistance {32.0};
        const float maxDistance {39.0};
        float perfectUpDistance {22.0};
        float upTargetDistance {16.0};          // se le suma a la posicion del jugador para establecer el target

        float actualDistance {34.0};            // esto es la posicion de la camara actual en los ejes x-z
        float upDistance {20.0};                // en caso de querer modificar el alto de alguna forma en un futuro
        float collisionDistance {0.0};          // en caso de colisionar contra una pared ajustarla restandole esto

        const float camAceleration {12.0 * Constants::DELTA_TIME};
        const float camDeceleration {6.0 * Constants::DELTA_TIME};

        bool wheelCamera{true};
        const float heightWheelCam{6.0};
        const float posXWheelCam{9.0};
        const float posZWheelCam{9.0};
    private:

};
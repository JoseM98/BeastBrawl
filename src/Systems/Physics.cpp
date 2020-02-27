#include "Physics.h"
#include <Components/CCamera.h>
#include <Components/CCar.h>
#include <Components/CNitro.h>
#include <Components/CSpeed.h>
#include <Entities/Camera.h>
#include <Entities/Car.h>
#include "../Components/CExternalForce.h"
#include "../Components/COnline.h"
#include "../Entities/CarHuman.h"
#include "../Systems/Utils.h"

#include <cmath>

#include <math.h>
#include <iostream>

#include "../Constants.h"

Physics::Physics() {
}

void Physics::UpdateEveryFrame(Car *car, Camera *cam, double percentTick) {
    // actualizar posiciones
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cTransformableCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());

    // std::cout << "speed:" << cCar->speed << " ( " << cTransformableCam->positionPrev.z << " , " << cTransformableCam->positionNext.z << " , " << cTransformableCam->position.z << " ) --> " << percentTick << std::endl;
    cTransformable->position.x = (cTransformable->positionNext.x - cTransformable->positionPrev.x) * percentTick + cTransformable->positionPrev.x;
    cTransformable->position.y = (cTransformable->positionNext.y - cTransformable->positionPrev.y) * percentTick + cTransformable->positionPrev.y;
    cTransformable->position.z = (cTransformable->positionNext.z - cTransformable->positionPrev.z) * percentTick + cTransformable->positionPrev.z;

    // cout << "posCar:" << cTransformable->position.x << ",\t" << cTransformable->position.y << ",\t" << cTransformable->position.z << endl;
    // inicio HACK
    // aquí solucionamos que al pasar de 0º -> 355º, hace efectivamente eso, 0 a 355, en vez de 360 a 355,
    // por tanto el salto lo hace así:
    //      rotation.y( 0 , 355 , 1.9525 )
    //      rotation.y( 0 , 355 , 89.6644 )
    //      rotation.y( 0 , 355 , 179.173 )
    //      rotation.y( 0 , 355 , 269.052 )
    //      rotation.y( 0 , 355 , 355 )
    // en vez de así:
    //      rotation.y( 360 , 355 , 360 )
    //      rotation.y( 360 , 355 , 358,75 )
    //      rotation.y( 360 , 355 , 357,5 )
    //      rotation.y( 360 , 355 , 356,25 )
    //      rotation.y( 360 , 355 , 355 )
    // Tenemos el mismo problema cuando pasamos de 360º a 5º
    // si pasamos de alrededor de 355 a 5...
    float auxNext = cTransformable->rotationNext.y;
    float auxPrev = cTransformable->rotationPrev.y;
    if (cTransformable->rotationNext.y >= 360 - 15 && cTransformable->rotationPrev.y <= 15) {
        auxPrev += 360;
    } else if (cTransformable->rotationPrev.y >= 360 - 15 && cTransformable->rotationNext.y <= 15) {
        auxPrev -= 360;
    }
    cTransformable->rotation.y = (auxNext - auxPrev) * percentTick + auxPrev;
    // std::cout << "rotation.y( " << cTransformable->rotationPrev.y << " , " << cTransformable->rotationNext.y << " , " << cTransformable->rotation.y << " ) --> " << percentTick << std::endl;
    // fin HACK

    // DEBUG_CAMERA
    cCamera->rotExtraY = (cCamera->rotExtraYNext - cCamera->rotExtraYPrev) * percentTick + cCamera->rotExtraYPrev;
    // cout << "rotExtraY: " << cCamera->rotExtraY << endl;
    // CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);

    // cTransformableCam->position.y = cTransformable->position.y + 20;
    // cTransformableCam->position.x = (cTransformable->position.x + 40 * cos(((cTransformable->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
    // cTransformableCam->position.z = (cTransformable->position.z - 40 * sin(((cTransformable->rotation.y - cCamera->rotExtraY) * PI) / 180.0));

    // cTransformableCam->position.x = (cTransformableCam->positionNext.x - cTransformableCam->positionPrev.x) * percentTick + cTransformableCam->positionPrev.x;
    // cTransformableCam->position.y = (cTransformableCam->positionNext.y - cTransformableCam->positionPrev.y) * percentTick + cTransformableCam->positionPrev.y;
    // cTransformableCam->position.z = (cTransformableCam->positionNext.z - cTransformableCam->positionPrev.z) * percentTick + cTransformableCam->positionPrev.z;

    // cTransformableCam->rotation.y = (cTransformableCam->rotationNext.y - cTransformableCam->rotationPrev.y) * percentTick + cTransformableCam->rotationPrev.y;
    // cout << "posCam:" << cTransformableCam->position.x << ",\t" << cTransformableCam->position.y << ",\t" << cTransformableCam->position.z << endl;
    // cout << "resta:" << cTransformable->position.x - cTransformableCam->position.x
    //      << ",\t" << cTransformable->position.y - cTransformableCam->position.y
    //      << ",\t" << cTransformable->position.z - cTransformableCam->position.z << endl;

    // glm::vec3 resta = cTransformable->position - cTransformableCam->position;
    // cout << "length:" << glm::distance(cTransformable->position, cTransformableCam->position) << endl;

    // std::cout << "rotation.y( " << cTransformableCam->rotationPrev.y << " , " << cTransformableCam->rotationNext.y << " , " << cTransformableCam->rotation.y << " ) --> " << percentTick << std::endl;
}

void Physics::update(Car *car, Camera *cam) {
    // actualizar posiciones
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());
    auto cTransformableCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cExternalForce = static_cast<CExternalForce *>(car->GetComponent(CompType::CompExternalForce).get());

    //std::cout << "( " << cTransformableCam->position.z << " )" << std::endl;
    cTransformable->positionPrev = cTransformable->positionNext;
    cTransformable->position = cTransformable->positionNext;
    cTransformableCam->positionPrev = cTransformableCam->positionNext;
    cTransformableCam->position = cTransformableCam->positionNext;

    cTransformable->rotationPrev = cTransformable->rotationNext;
    cTransformable->rotation = cTransformable->rotationNext;

    // cCamera->rotExtraY = cCamera->rotExtraYNext;
    // cCamera->rotExtraYPrev = cCamera->rotExtraYNext;

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, cSpeed, cExternalForce);
    else
        CalculatePositionReverse(cCar, cTransformable, cExternalForce);

    // DEBUG_CAMERA
    // CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);
    //std::cout << "( " << cTransformableCam->position.z << " )" << std::endl;
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, CExternalForce *cExternalForce) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;

    // debemos de tener encuenta la fuerza externa, asi como la direccion final que tomaremos (el angulo final)
    if (cExternalForce->force > 0) {
        // Este paso es una tonteria porque ya lo devolvemos normalizado
        cExternalForce->dirExternalForce = normalize(cExternalForce->dirExternalForce);
    }

    // Movimiento del coche
    cSpeed->speed.x = cos(angleRotation);  // * cCar->speed;
    cSpeed->speed.z = sin(angleRotation);  // * cCar->speed;
    cSpeed->speed.y = 0.f;                 // TODO, esto lo cacharreará el CLPhysics

    cTransformable->positionNext.x = cTransformable->positionPrev.x - cSpeed->speed.x * cCar->speed * Constants::DELTA_TIME;
    cTransformable->positionNext.y = cTransformable->positionPrev.y - cSpeed->speed.y * cCar->speed * Constants::DELTA_TIME;
    cTransformable->positionNext.z = cTransformable->positionPrev.z + cSpeed->speed.z * cCar->speed * Constants::DELTA_TIME;

    // Rotacion del coche
    // if (cCar->wheelRotation != 0) {
    // cTransformable->rotation.y += cCar->wheelRotation * 0.20;
    cTransformable->rotationNext.y = cTransformable->rotationPrev.y + cCar->wheelRotation * Constants::DELTA_TIME;

    if (cTransformable->rotationNext.y >= 360.0)
        cTransformable->rotationNext.y -= 360.0;
    else if (cTransformable->rotationNext.y < 0.0)
        cTransformable->rotationNext.y += 360.0;
    // cout << "Current wheel rotation: " << cCar->wheelRotation << "; Current rotationNext.y: " << cTransformable->rotationNext.y << endl;

    // }
}

vec3 Physics::CalculateVecDirCar(CTransformable *cTransformable) const {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float nextPosX = cTransformable->position.x - cos(angleRotation) * 1;
    float nexPosZ = cTransformable->position.z + sin(angleRotation) * 1;

    return vec3(nextPosX - cTransformable->position.x, 0, nexPosZ - cTransformable->position.z);
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, CExternalForce *cExternalForce) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;

    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->positionNext.z = cTransformable->positionPrev.z + sin(angleRotation) * cCar->speed * Constants::DELTA_TIME;
    cTransformable->positionNext.y = cTransformable->positionPrev.y + sin(angleRotation) * cCar->speed * Constants::DELTA_TIME;
    cTransformable->positionNext.x = cTransformable->positionPrev.x - cos(angleRotation) * cCar->speed * Constants::DELTA_TIME;

    cTransformable->rotationNext.y = cTransformable->rotationPrev.y - cCar->wheelRotation * Constants::DELTA_TIME;

    //Si tiene rotacion, rotamos el coche
    if (cCar->wheelRotation != 0) {
        cTransformable->rotation.y -= cCar->wheelRotation * 0.20;
        if (cTransformable->rotation.y >= 360.0)
            cTransformable->rotation.y -= 360.0;
        else if (cTransformable->rotation.y < 0.0)
            cTransformable->rotation.y += 360.0;
    }
}

//Calcula la posicion de la camara (duda con las formulas preguntar a Jose)
void Physics::CalculatePositionCamera(CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera) {
    cTransformableCamera->position.y = cTransformableCar->position.y + 20;
    cTransformableCamera->position.x = (cTransformableCar->position.x + 40 * cos(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
    cTransformableCamera->position.z = (cTransformableCar->position.z - 40 * sin(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));

    cout << "cCam Rot Extra " << cCamera->rotExtraY << endl;
}

//Entra cuando se presiona la I
void Physics::Accelerate(Car *car, Camera *cam) {
    // versión anterior
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    //auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Aumentamos la velocidad
    if (cNitro->activePowerUp == false) {
        cCar->speed += cCar->acceleration;
        if (cCar->speed > cCar->maxSpeed) {
            cCar->speed -= cCar->acceleration * 4.0;
            if (cCar->speed < cCar->maxSpeed)
                cCar->speed = cCar->maxSpeed;
        }
    } else {
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }

    // versión nueva
    /*auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Aumentamos la velocidad
    if (cNitro->activePowerUp == false) {
        cSpeed->speed += cCar->acceleration;
        if (cSpeed->speed > cCar->maxSpeed) {
            cSpeed->speed -= cCar->acceleration * 4.0;
            if (cSpeed->speed < cCar->maxSpeed)
                cSpeed->speed = cCar->maxSpeed;
        }
    } else {
        cSpeed->speed += cNitro->nitroAcceleration;
        if (cSpeed->speed > cNitro->nitroMaxSpeed) {
            cSpeed->speed = cNitro->nitroMaxSpeed;
        }
    }*/
}

//Entra cuando se presiona la O
void Physics::Decelerate(Car *car, Camera *cam) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    //Reducimos la velocidad
    if (cNitro->activePowerUp == false) {
        cCar->speed -= cCar->slowDown;
        if (cCar->speed < cCar->reverseMaxSpeed) {  // no se supere la velocidad marcha atras
            cCar->speed = cCar->reverseMaxSpeed;
        }
    } else {
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }
}

//Entra cuando se presiona la A
/*void Physics::Turn(Car *car, Camera *cam, bool right) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());
    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());

    int8_t multiplicador = -1;
    if(right) 
        multiplicador = 1;

    cCamera->rotExtraYPrev = cCamera->rotExtraYNext;
    cCamera->rotExtraY = cCamera->rotExtraYNext;
    cout << "antes: prev[" << cCamera->rotExtraYPrev << "] next[" << cCamera->rotExtraYNext << "] current[" << cCamera->rotExtraY << "]" << endl;

    if (cCar->speed >= cCar->maxSpeed * 0.15) {
        if ( (!right && cCar->wheelRotation > -cCar->maxWheelRotation) 
            || (right && cCar->wheelRotation < cCar->maxWheelRotation)) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation += cCar->incrementWheelRotation * multiplicador;
        } 

        if (cCamera->rotExtraYNext > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraYNext += cCar->incrementWheelRotation * Constants::DELTA_TIME * multiplicador;
        }
    } else if (cCar->speed <= -cCar->maxSpeed * 0.15) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation += cCar->incrementWheelRotation * multiplicador;
        }
        if (cCamera->rotExtraYNext > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraYNext += cCar->incrementWheelRotation * Constants::DELTA_TIME * multiplicador;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
        if (cCar->wheelRotation >= cCar->decrementWheelRotation) {
            cCar->wheelRotation -= cCar->decrementWheelRotation;
        } else if (cCar->wheelRotation <= -cCar->decrementWheelRotation) {
            cCar->wheelRotation += cCar->decrementWheelRotation;
        } else {
            cCar->wheelRotation = 0;
        }

        if (cCamera->rotExtraYNext >= cCar->decrementWheelRotation) {
            cCamera->rotExtraYNext -= cCar->decrementWheelRotation;
        } else if (cCamera->rotExtraYNext <= -cCar->decrementWheelRotation) {
            cCamera->rotExtraYNext += cCar->decrementWheelRotation;
        } else {
            cCamera->rotExtraYNext = 0;
        }
    }
    cout << "después: prev[" << cCamera->rotExtraYPrev << "] next[" << cCamera->rotExtraYNext << "] current[" << cCamera->rotExtraY << "]" << endl;
}*/

void Physics::TurnLeft(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    cCamera->rotExtraYPrev = cCamera->rotExtraYNext;
    cCamera->rotExtraY = cCamera->rotExtraYNext;
    cout << "antes: prev[" << cCamera->rotExtraYPrev << "] next[" << cCamera->rotExtraYNext << "] current[" << cCamera->rotExtraY << "]" << endl;

    if (cCar->speed >= cCar->maxSpeed * 0.15) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }

        if (cCamera->rotExtraYNext > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraYNext -= cCar->incrementWheelRotation * Constants::DELTA_TIME;
        }
    } else if (cCar->speed <= -cCar->maxSpeed * 0.15) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }
        if (cCamera->rotExtraYNext > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraYNext -= cCar->incrementWheelRotation * Constants::DELTA_TIME;
        }
    } else {
        // la rueda vuelve a su sitio original al no dejarte rotar
        RepositionWheelInCenter(cCar, cCamera);
    }
    cout << "después: prev[" << cCamera->rotExtraYPrev << "] next[" << cCamera->rotExtraYNext << "] current[" << cCamera->rotExtraY << "]" << endl;
}

//Entra cuando se presiona la D
void Physics::TurnRight(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    cCamera->rotExtraYPrev = cCamera->rotExtraYNext;
    cCamera->rotExtraY = cCamera->rotExtraYNext;
    cout << "antes: prev[" << cCamera->rotExtraYPrev << "] next[" << cCamera->rotExtraYNext << "] current[" << cCamera->rotExtraY << "]" << endl;

    if (cCar->speed >= cCar->maxSpeed * 0.15) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }

        if (cCamera->rotExtraYNext < (cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraYNext += cCar->incrementWheelRotation * Constants::DELTA_TIME;
        }
    } else if (cCar->speed <= -cCar->maxSpeed * 0.15) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }
        if (cCamera->rotExtraYNext < (cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraYNext += cCar->incrementWheelRotation * Constants::DELTA_TIME;
        }
    } else {
        // la rueda vuelve a su sitio original al no dejarte rotar
        RepositionWheelInCenter(cCar, cCamera);
    }
    cout << "después: prev[" << cCamera->rotExtraYPrev << "] next[" << cCamera->rotExtraYNext << "] current[" << cCamera->rotExtraY << "]" << endl;
}

void Physics::RepositionWheelInCenter(CCar *cCar, CCamera *cCamera) {
    if (cCar->wheelRotation >= cCar->decrementWheelRotation) {
        cCar->wheelRotation -= cCar->decrementWheelRotation;
    } else if (cCar->wheelRotation <= -cCar->decrementWheelRotation) {
        cCar->wheelRotation += cCar->decrementWheelRotation;
    } else {
        cCar->wheelRotation = 0;
    }

    if (cCamera->rotExtraY >= cCar->decrementWheelRotation) {
        cCamera->rotExtraY -= cCar->decrementWheelRotation;
    } else if (cCamera->rotExtraY <= -cCar->decrementWheelRotation) {
        cCamera->rotExtraY += cCar->decrementWheelRotation;
    } else {
        cCamera->rotExtraY = 0;
    }
}

//Aqui entra cuando no se esta presionando ni I ni O
void Physics::NotAcceleratingOrDecelerating(Car *car, Camera *cam) {
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());

    if (cNitro->activePowerUp == false) {
        if (cCar->speed > 0) {
            cCar->speed -= cCar->friction;
            if (cCar->speed < 0)
                cCar->speed = 0;
        } else if (cCar->speed < 0) {
            cCar->speed += cCar->friction;
            if (cCar->speed > 0)
                cCar->speed = 0;
        }
    } else {
        cCar->speed += cNitro->nitroAcceleration;
        if (cCar->speed > cNitro->nitroMaxSpeed) {
            cCar->speed = cNitro->nitroMaxSpeed;
        }
    }
}

void Physics::FrictionExternalForce(CCar *cCar, CExternalForce *externalForce) const {
    externalForce->force -= cCar->friction;
}

//Aqui entra cuando no se esta presionando ni A ni D
void Physics::NotTurning(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    cCamera->rotExtraYPrev = cCamera->rotExtraYNext;
    cCamera->rotExtraY = cCamera->rotExtraYNext;
    cout << "antes: prev[" << cCamera->rotExtraYPrev << "] next[" << cCamera->rotExtraYNext << "] current[" << cCamera->rotExtraY << "]" << endl;

    if (cCar->wheelRotation >= cCar->decrementWheelRotation) {
        cCar->wheelRotation -= cCar->decrementWheelRotation;
    } else if (cCar->wheelRotation <= -cCar->decrementWheelRotation) {
        cCar->wheelRotation += cCar->decrementWheelRotation;
    } else {
        cCar->wheelRotation = 0;
    }

    if (cCamera->rotExtraYNext >= cCar->decrementWheelRotation) {
        cCamera->rotExtraYNext -= cCar->decrementWheelRotation;
    } else if (cCamera->rotExtraYNext <= -cCar->decrementWheelRotation) {
        cCamera->rotExtraYNext += cCar->decrementWheelRotation;
    } else {
        cCamera->rotExtraYNext = 0;
    }
    cout << "antes: prev[" << cCamera->rotExtraYPrev << "] next[" << cCamera->rotExtraYNext << "] current[" << cCamera->rotExtraY << "]" << endl;
}

void Physics::UpdateHuman(Car *car) {
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cNitro = static_cast<CNitro *>(car->GetComponent(CompType::NitroComp).get());
    auto cOnline = static_cast<COnline *>(car->GetComponent(CompType::OnlineComp).get());
    auto cExternalForce = static_cast<CExternalForce *>(car->GetComponent(CompType::CompExternalForce).get());

    bool accDec = false;
    bool turning = false;
    // actualizar inputs
    for (Constants::InputTypes input : cOnline->inputs) {
        if (input == Constants::InputTypes::FORWARD) {
            AccelerateHuman(*cCar, *cNitro);
            accDec = true;
        } else if (input == Constants::InputTypes::BACK) {
            DecelerateHuman(*cCar, *cNitro);
            accDec = true;
        } else if (input == Constants::InputTypes::LEFT) {
            TurnLeftHuman(*cCar);
            turning = true;
        } else if (input == Constants::InputTypes::RIGHT) {
            TurnRightHuman(*cCar);
            turning = true;
        } else if (input == Constants::InputTypes::CLAXON) {
            // evento
        } else if (input == Constants::InputTypes::LAUNCH_PU) {
            shared_ptr<DataMap> data = make_shared<DataMap>();

            (*data)[DataType::ACTUAL_CAR] = car;
            EventManager::GetInstance().AddEventMulti(Event{EventType::THROW_POWERUP_HUMAN, data});
        }
    }
    if (accDec == false)
        NotAcceleratingOrDeceleratingHuman(*cCar, *cNitro);
    if (turning == false)
        NotTurningHuman(*cCar);

    // actualizar posiciones
    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, cSpeed, cExternalForce);
    else
        CalculatePositionReverse(cCar, cTransformable, cExternalForce);
}

//Entra cuando se presiona la I
void Physics::AccelerateHuman(CCar &cCar, CNitro &cNitro) const {
    //Aumentamos la velocidad
    if (cNitro.activePowerUp == false) {
        cCar.speed += cCar.acceleration;
        if (cCar.speed > cCar.maxSpeed) {
            cCar.speed -= cCar.acceleration * 4.0;
            if (cCar.speed < cCar.maxSpeed)
                cCar.speed = cCar.maxSpeed;
        }
    } else {
        cCar.speed += cNitro.nitroAcceleration;
        if (cCar.speed > cNitro.nitroMaxSpeed) {
            cCar.speed = cNitro.nitroMaxSpeed;
        }
    }
}

//Entra cuando se presiona la O
void Physics::DecelerateHuman(CCar &cCar, CNitro &cNitro) const {
    //Reducimos la velocidad
    if (cNitro.activePowerUp == false) {
        cCar.speed -= cCar.slowDown;
        if (cCar.speed < cCar.reverseMaxSpeed) {  // no se supere la velocidad marcha atras
            cCar.speed = cCar.reverseMaxSpeed;
        }
    } else {
        cCar.speed += cNitro.nitroAcceleration;
        if (cCar.speed > cNitro.nitroMaxSpeed) {
            cCar.speed = cNitro.nitroMaxSpeed;
        }
    }
}

//Entra cuando se presiona la A
void Physics::TurnLeftHuman(CCar &cCar) const {
    if (cCar.speed >= cCar.maxSpeed * 0.15) {
        if (cCar.wheelRotation > -cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar.wheelRotation -= cCar.incrementWheelRotation;
        }
    } else if (cCar.speed <= -cCar.maxSpeed * 0.15) {
        if (cCar.wheelRotation > -cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar.wheelRotation -= cCar.incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
        if (cCar.wheelRotation >= cCar.decrementWheelRotation) {
            cCar.wheelRotation -= cCar.decrementWheelRotation;
        } else if (cCar.wheelRotation <= -cCar.decrementWheelRotation) {
            cCar.wheelRotation += cCar.decrementWheelRotation;
        } else {
            cCar.wheelRotation = 0;
        }
    }
}

//Entra cuando se presiona la D
void Physics::TurnRightHuman(CCar &cCar) const {
    if (cCar.speed >= cCar.maxSpeed * 0.15) {
        if (cCar.wheelRotation < cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar.wheelRotation += cCar.incrementWheelRotation;
        }
    } else if (cCar.speed <= -cCar.maxSpeed * 0.15) {
        if (cCar.wheelRotation < cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar.wheelRotation += cCar.incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
        if (cCar.wheelRotation >= cCar.decrementWheelRotation) {
            cCar.wheelRotation -= cCar.decrementWheelRotation;
        } else if (cCar.wheelRotation <= -cCar.decrementWheelRotation) {
            cCar.wheelRotation += cCar.decrementWheelRotation;
        } else {
            cCar.wheelRotation = 0;
        }
    }
}

//Aqui entra cuando no se esta presionando ni I ni O
void Physics::NotAcceleratingOrDeceleratingHuman(CCar &cCar, CNitro &cNitro) const {
    if (cNitro.activePowerUp == false) {
        if (cCar.speed > 0) {
            cCar.speed -= cCar.friction;
            if (cCar.speed < 0)
                cCar.speed = 0;
        } else if (cCar.speed < 0) {
            cCar.speed += cCar.friction;
            if (cCar.speed > 0)
                cCar.speed = 0;
        }
    } else {
        cCar.speed += cNitro.nitroAcceleration;
        if (cCar.speed > cNitro.nitroMaxSpeed) {
            cCar.speed = cNitro.nitroMaxSpeed;
        }
    }
}

//Aqui entra cuando no se esta presionando ni A ni D
void Physics::NotTurningHuman(CCar &cCar) const {
    if (cCar.wheelRotation >= cCar.decrementWheelRotation) {
        cCar.wheelRotation -= cCar.decrementWheelRotation;
    } else if (cCar.wheelRotation <= -cCar.decrementWheelRotation) {
        cCar.wheelRotation += cCar.decrementWheelRotation;
    } else {
        cCar.wheelRotation = 0;
    }
}
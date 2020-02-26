#include "Physics.h"
#include <Components/CCamera.h>
#include <Components/CCar.h>
#include <Components/CNitro.h>
#include <Components/CSpeed.h>
#include <Entities/Camera.h>
#include <Entities/Car.h>
#include "../Entities/CarHuman.h"
#include "../Components/COnline.h"
#include "../Components/CExternalForce.h"
#include "../Systems/Utils.h"

#include <cmath>

#include <iostream>
#include <math.h>

#include "../Constants.h"

Physics::Physics(float _deltaTime) : deltaTime(_deltaTime) {
}


void Physics::UpdateEveryFrame(Car* car, Camera* cam, double percentTick){
    // actualizar posiciones
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cTransformableCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());

    // std::cout << "speed:" << cCar->speed << " ( " << cTransformableCam->positionPrev.z << " , " << cTransformableCam->positionNext.z << " , " << cTransformableCam->position.z << " ) --> " << percentTick << std::endl;
    cTransformable->position.x = (cTransformable->positionNext.x - cTransformable->positionPrev.x) * percentTick + cTransformable->positionPrev.x;
    cTransformable->position.y = (cTransformable->positionNext.y - cTransformable->positionPrev.y) * percentTick + cTransformable->positionPrev.y;
    cTransformable->position.z = (cTransformable->positionNext.z - cTransformable->positionPrev.z) * percentTick + cTransformable->positionPrev.z;
    
    cTransformable->rotation.y = (cTransformable->rotationNext.y - cTransformable->rotationPrev.y) * percentTick + cTransformable->rotationPrev.y;
    std::cout << "rotation.y( " << cTransformable->rotationPrev.y << " , " << cTransformable->rotationNext.y << " , " << cTransformable->rotation.y << " ) --> " << percentTick << std::endl;


    cTransformableCam->position.x = (cTransformableCam->positionNext.x - cTransformableCam->positionPrev.x) * percentTick + cTransformableCam->positionPrev.x;
    cTransformableCam->position.y = (cTransformableCam->positionNext.y - cTransformableCam->positionPrev.y) * percentTick + cTransformableCam->positionPrev.y;
    cTransformableCam->position.z = (cTransformableCam->positionNext.z - cTransformableCam->positionPrev.z) * percentTick + cTransformableCam->positionPrev.z;
    
    cTransformableCam->rotation.y = (cTransformableCam->rotationNext.y - cTransformableCam->rotationPrev.y) * percentTick + cTransformableCam->rotationPrev.y;
    
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

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, cSpeed, cExternalForce, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, cExternalForce, deltaTime);

    // DEBUG_CAMERA
    // CalculatePositionCamera(cTransformable, cTransformableCam, cCamera);
    //std::cout << "( " << cTransformableCam->position.z << " )" << std::endl;
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, CExternalForce *cExternalForce, float deltaTime) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;

    // debemos de tener encuenta la fuerza externa, asi como la direccion final que tomaremos (el angulo final)
    if(cExternalForce->force > 0){
        // Este paso es una tonteria porque ya lo devolvemos normalizado
        cExternalForce->dirExternalForce = normalize(cExternalForce->dirExternalForce);
    }

    // Movimiento del coche
    cSpeed->speed.x = cos(angleRotation);  // * cCar->speed;
    cSpeed->speed.z = sin(angleRotation);  // * cCar->speed;
    cSpeed->speed.y = 0.f;                 // TODO, esto lo cacharreará el CLPhysics

    cTransformable->positionNext.x = cTransformable->positionPrev.x - cSpeed->speed.x * cCar->speed * deltaTime;
    cTransformable->positionNext.y = cTransformable->positionPrev.y - cSpeed->speed.y * cCar->speed * deltaTime;
    cTransformable->positionNext.z = cTransformable->positionPrev.z + cSpeed->speed.z * cCar->speed * deltaTime;

    

    // Rotacion del coche
    // if (cCar->wheelRotation != 0) {
    // cTransformable->rotation.y += cCar->wheelRotation * 0.20;
    cTransformable->rotationNext.y = cTransformable->rotationPrev.y + cCar->wheelRotation * Constants::DELTA_TIME;

    if (cTransformable->rotationNext.y >= 360.0)
        cTransformable->rotationNext.y -= 360.0;
    else if (cTransformable->rotationNext.y < 0.0)
        cTransformable->rotationNext.y += 360.0;
    cout << "Current wheel rotation: " << cCar->wheelRotation << "; Current rotationNext.y: " << cTransformable->rotationNext.y << endl;

    // }
}

vec3 Physics::CalculateVecDirCar(CTransformable *cTransformable) const{

   float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
   float nextPosX    = cTransformable->position.x - cos(angleRotation) * 1;
   float nexPosZ     = cTransformable->position.z + sin(angleRotation) * 1;

   return vec3(nextPosX-cTransformable->position.x, 0, nexPosZ-cTransformable->position.z);

}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, CExternalForce *cExternalForce, float deltaTime) {
    float angleRotation = (cTransformable->rotation.y * PI) / 180.0;
    float delta = deltaTime;

    //Modificamos la posicion en X y Z en funcion del angulo
    cTransformable->positionNext.z = cTransformable->positionPrev.z + sin(angleRotation) * cCar->speed * delta;
    cTransformable->positionNext.y = cTransformable->positionPrev.y + sin(angleRotation) * cCar->speed * delta;
    cTransformable->positionNext.x = cTransformable->positionPrev.x - cos(angleRotation) * cCar->speed * delta;

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
    // comento la primera linea porque la pos de la cámara en altura (por ahora) es siempre la misma
    cTransformableCamera->positionNext.y = cTransformableCar->positionNext.y + 20;
    cTransformableCamera->positionNext.x = (cTransformableCar->positionNext.x + 40 * cos(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
    cTransformableCamera->positionNext.z = (cTransformableCar->positionNext.z - 40 * sin(((cTransformableCar->rotation.y - cCamera->rotExtraY) * PI) / 180.0));
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
void Physics::TurnLeft(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera*>(cam->GetComponent(CompType::CameraComp).get());
    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());

    std::cout << "Gira derecha: " << cTrans->rotation.y << "\n";


    if (cCar->speed >= cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }

        if (cCamera->rotExtraY > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY -= cCar->incrementWheelRotation;
        }
    } else if (cCar->speed <= -cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation > -cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar->wheelRotation -= cCar->incrementWheelRotation;
        }
        if (cCamera->rotExtraY > -(cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY -= cCar->incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
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
}

//Entra cuando se presiona la D
void Physics::TurnRight(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if (cCar->speed >= cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }

        if (cCamera->rotExtraY < (cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY += cCar->incrementWheelRotation;
        }
    } else if (cCar->speed <= -cCar->maxSpeed*0.15) {
        if (cCar->wheelRotation < cCar->maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar->wheelRotation += cCar->incrementWheelRotation;
        }
        if (cCamera->rotExtraY < (cCar->maxWheelRotation + cCamera->rotExtraCamera)) {
            cCamera->rotExtraY += cCar->incrementWheelRotation;
        }
    } else {  // la rueda vuelve a su sitio original al no dejarte rotar
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


void Physics::FrictionExternalForce(CCar *cCar, CExternalForce *externalForce) const{
    externalForce->force -= cCar->friction;
}


//Aqui entra cuando no se esta presionando ni A ni D
void Physics::NotTurning(Car *car, Camera *cam) {
    //Componentes de la camara
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());

    //Componentes del coche
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

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
    for(Constants::InputTypes input : cOnline->inputs){
        if(input == Constants::InputTypes::FORWARD){
            AccelerateHuman(*cCar, *cNitro);
            accDec = true;
        }else if(input == Constants::InputTypes::BACK){
            DecelerateHuman(*cCar, *cNitro);
            accDec = true;
        }else if(input == Constants::InputTypes::LEFT){
            TurnLeftHuman(*cCar);
            turning = true;
        }else if(input == Constants::InputTypes::RIGHT){
            TurnRightHuman(*cCar);
            turning = true;
        }else if(input == Constants::InputTypes::CLAXON){
            // evento
        }else if(input == Constants::InputTypes::LAUNCH_PU){
            shared_ptr<DataMap> data = make_shared<DataMap>();

            (*data)[DataType::ACTUAL_CAR] = car;
            EventManager::GetInstance().AddEventMulti(Event{EventType::THROW_POWERUP_HUMAN, data});
        }
    }
    if(accDec == false)
        NotAcceleratingOrDeceleratingHuman(*cCar, *cNitro);
    if(turning == false)
        NotTurningHuman(*cCar);


    // actualizar posiciones
    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, cSpeed, cExternalForce, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, cExternalForce, deltaTime);
}




//Entra cuando se presiona la I
void Physics::AccelerateHuman(CCar &cCar, CNitro &cNitro) const{
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
void Physics::DecelerateHuman(CCar &cCar, CNitro &cNitro) const{
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
void Physics::TurnLeftHuman(CCar &cCar) const{
    if (cCar.speed >= cCar.maxSpeed*0.15) {
        if (cCar.wheelRotation > -cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la izquierda
            cCar.wheelRotation -= cCar.incrementWheelRotation;
        }
    } else if (cCar.speed <= -cCar.maxSpeed*0.15) {
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
void Physics::TurnRightHuman(CCar &cCar) const{
    if (cCar.speed >= cCar.maxSpeed*0.15) {
        if (cCar.wheelRotation < cCar.maxWheelRotation) {
            //Aumentamos la rotacion hacia la derecha
            cCar.wheelRotation += cCar.incrementWheelRotation;
        }
    } else if (cCar.speed <= -cCar.maxSpeed*0.15) {
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
void Physics::NotAcceleratingOrDeceleratingHuman(CCar &cCar, CNitro &cNitro) const{
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
void Physics::NotTurningHuman(CCar &cCar) const{
    if (cCar.wheelRotation >= cCar.decrementWheelRotation) {
        cCar.wheelRotation -= cCar.decrementWheelRotation;
    } else if (cCar.wheelRotation <= -cCar.decrementWheelRotation) {
        cCar.wheelRotation += cCar.decrementWheelRotation;
    } else {
        cCar.wheelRotation = 0;
    }
}
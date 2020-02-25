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

void Physics::update(Car *car, Camera *cam) {
    // actualizar posiciones
    auto cTransformable = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());
    auto cCamera = static_cast<CCamera *>(cam->GetComponent(CompType::CameraComp).get());
    auto cTransformableCam = static_cast<CTransformable *>(cam->GetComponent(CompType::TransformableComp).get());
    auto cSpeed = static_cast<CSpeed *>(car->GetComponent(CompType::SpeedComp).get());
    auto cExternalForce = static_cast<CExternalForce *>(car->GetComponent(CompType::CompExternalForce).get());

    if (cCar->speed >= 0)
        CalculatePosition(cCar, cTransformable, cSpeed, cExternalForce, deltaTime);
    else
        CalculatePositionReverse(cCar, cTransformable, cExternalForce, deltaTime);
    CalculatePositionCamera(cCar, cTransformable, cTransformableCam, cCamera);
}

//Calcula la posicion del coche (duda con las formulas preguntar a Jose)
void Physics::CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, CExternalForce *cExternalForce, float deltaTime) {

    // debemos de tener encuenta la fuerza externa, asi como la direccion final que tomaremos (el angulo final)
    if(cExternalForce->force > 0){
        // Este paso es una tonteria porque ya lo devolvemos normalizado
        cExternalForce->dirExternalForce = normalize(cExternalForce->dirExternalForce);
    }


    float angleRotation = cTransformable->rotation.y - cCar->skidRotation;
    if(angleRotation < 0) angleRotation += 360.0;
    if(angleRotation >= 360) angleRotation -= 360.0;
    float angleRotationFinal = (angleRotation * PI) / 180.0;

    // Movimiento del coche
    cSpeed->speed.x = cos(angleRotationFinal);  // * cCar->speed;
    cSpeed->speed.z = sin(angleRotationFinal);  // * cCar->speed;
    cSpeed->speed.y = 0.f;                 // TODO, esto lo cacharreará el CLPhysics
    cTransformable->position.x -= cSpeed->speed.x * cCar->speed * deltaTime;
    cTransformable->position.z += cSpeed->speed.z * cCar->speed * deltaTime;

    

    // Rotacion del coche
    // if (cCar->wheelRotation != 0) {
    if(cCar->skidRotation != 0)
        cTransformable->rotation.y += cCar->wheelRotation * 0.05 + 0.022 * cCar->skidRotation;
    else
        cTransformable->rotation.y += cCar->wheelRotation * 0.20;


    if (cTransformable->rotation.y >= 360.0)
        cTransformable->rotation.y -= 360.0;
    else if (cTransformable->rotation.y < 0.0)
        cTransformable->rotation.y += 360.0;

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
    cTransformable->position.z += sin(angleRotation) * cCar->speed * delta;
    cTransformable->position.x -= cos(angleRotation) * cCar->speed * delta;

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
void Physics::CalculatePositionCamera(CCar *cCar, CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera) {
    // comento la primera linea porque la pos de la cámara en altura (por ahora) es siempre la misma
    float rotationFinal = cTransformableCar->rotation.y - cCar->skidRotation - cCamera->rotExtraY;
    if (rotationFinal >= 360.0)
        rotationFinal -= 360.0;
    else if (rotationFinal < 0.0)
        rotationFinal += 360.0;
    cTransformableCamera->position.y = cTransformableCar->position.y + 20;
    cTransformableCamera->position.x = (cTransformableCar->position.x + 40 * cos(((rotationFinal) * PI) / 180.0));
    cTransformableCamera->position.z = (cTransformableCar->position.z - 40 * sin(((rotationFinal) * PI) / 180.0));
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


    /*if(cCar->skidState == SkidState::SKID_LEFT){
        auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        cCar->skidRotation -= cCar->skidAcc;
        cTrans->rotation.y -= cCar->skidAcc;
        if(cCar->skidRotation < cCar->skidDeg){
            cCar->skidRotation += cCar->skidAcc;
            cTrans->rotation.y += cCar->skidAcc;
        }
        if(cTrans->rotation.y < 0) cTrans->rotation.y += 360.0; 
    }*/
    if(cCar->skidState == SkidState::SKID_START && cCar->speed>cCar->maxSpeed*0.6 && duration_cast<milliseconds>(system_clock::now() - cCar->skidStart).count() < cCar->skidActivationTime){
        cCar->skidState = SkidState::SKID_TO_LEFT;
        cCar->skidDeg = cCar->skidDegL;
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

    if(cCar->skidState == SkidState::SKID_START && cCar->speed>cCar->maxSpeed*0.6 && duration_cast<milliseconds>(system_clock::now() - cCar->skidStart).count() < cCar->skidActivationTime){
        cCar->skidState = SkidState::SKID_TO_RIGHT;
        cCar->skidDeg = cCar->skidDegR;
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




// Metodo para cuando pulsamos el derrape
void Physics::Skid(Car *car, Camera *){
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    if(cCar->skidState == SkidState::DISABLED ){ // Reiniciamos el reloj y comenzamos, en caso de pulsar una de las dos direcciones dentro de un tiempo comenzara el giro del derrape
        cCar->skidStart = system_clock::now();
        cCar->skidState = SkidState::SKID_START;
    }/*else if(cCar->skidState == SkidState::SKID_START && duration_cast<milliseconds>(system_clock::now() - cCar->skidStart).count() > cCar->skidAnimationTime){
        cCar->skidState = SkidState::DISABLED;
    }*/

    // Si se mantiene pulsado va a incrementar hasta que se alcance la posicion
    if(cCar->skidState == SkidState::SKID_TO_LEFT){
        auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        cCar->skidRotation -= cCar->skidAcc;
        cTrans->rotation.y -= cCar->skidAcc;
        if(cCar->skidRotation < cCar->skidDeg){
            cCar->skidRotation += cCar->skidAcc;
            cTrans->rotation.y += cCar->skidAcc;
            cCar->skidState = SkidState::SKID_LEFT;
        }
        if(cTrans->rotation.y < 0) cTrans->rotation.y += 360.0;
    }else if(cCar->skidState == SkidState::SKID_TO_RIGHT){
        auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        cCar->skidRotation += cCar->skidAcc;
        cTrans->rotation.y += cCar->skidAcc;
        if(cCar->skidRotation > cCar->skidDeg){
            cCar->skidRotation -= cCar->skidAcc;
            cTrans->rotation.y -= cCar->skidAcc;
            cCar->skidState = SkidState::SKID_RIGHT;
        }
        if(cTrans->rotation.y >= 360) cTrans->rotation.y -= 360.0;  
    }
    if(cCar->speed<cCar->maxSpeed*0.5){
        if(cCar->skidState == SkidState::SKID_LEFT || cCar->skidState == SkidState::SKID_TO_LEFT)
            cCar->skidState = SkidState::SKID_RECOVER_LEFT;
        else if(cCar->skidState == SkidState::SKID_RIGHT || cCar->skidState == SkidState::SKID_TO_RIGHT)
            cCar->skidState = SkidState::SKID_RECOVER_RIGHT;
    }

    if( cCar->skidState == SkidState::SKID_RECOVER_LEFT){
        auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        cCar->skidRotation += cCar->skidAcc;
        cTrans->rotation.y += cCar->skidAcc;
        if(cCar->skidRotation >= 0){
            cCar->skidState = SkidState::SKID_START;
        }
        if(cTrans->rotation.y >= 360) cTrans->rotation.y -= 360.0;
    }else if(cCar->skidState == SkidState::SKID_RECOVER_RIGHT){
        auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        cCar->skidRotation -= cCar->skidAcc;
        cTrans->rotation.y -= cCar->skidAcc;
        if(cCar->skidRotation <= 0){
            cCar->skidState = SkidState::SKID_START;
        }
        if(cTrans->rotation.y < 0) cTrans->rotation.y += 360.0;
    }
}


void Physics::NotSkidding(Car *car, Camera *){
    auto cCar = static_cast<CCar *>(car->GetComponent(CompType::CarComp).get());

    // En caso de dejar de pulsar el derrape va a tener un periodo de recuperacion del derrape
    if( cCar->skidState == SkidState::SKID_LEFT || cCar->skidState == SkidState::SKID_TO_LEFT)
        cCar->skidState = SkidState::SKID_RECOVER_LEFT;
    else if( cCar->skidState == SkidState::SKID_RIGHT || cCar->skidState == SkidState::SKID_TO_RIGHT)
        cCar->skidState = SkidState::SKID_RECOVER_RIGHT;


    if( cCar->skidState == SkidState::SKID_RECOVER_LEFT){
        auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        cCar->skidRotation += cCar->skidAcc;
        cTrans->rotation.y += cCar->skidAcc;
        if(cCar->skidRotation >= 0){
            cCar->skidState = SkidState::DISABLED;
        }
        if(cTrans->rotation.y >= 360) cTrans->rotation.y -= 360.0;
    }else if(cCar->skidState == SkidState::SKID_RECOVER_RIGHT){
        auto cTrans = static_cast<CTransformable *>(car->GetComponent(CompType::TransformableComp).get());
        cCar->skidRotation -= cCar->skidAcc;
        cTrans->rotation.y -= cCar->skidAcc;
        if(cCar->skidRotation <= 0){
            cCar->skidState = SkidState::DISABLED;
        }
        if(cTrans->rotation.y < 0) cTrans->rotation.y += 360.0;
    }
    if(cCar->skidState == SkidState::SKID_START && duration_cast<milliseconds>(system_clock::now()-cCar->skidStart).count() > cCar->skidAnimationTime){
        cCar->skidState = SkidState::DISABLED;
        cCar->skidRotation = 0;
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
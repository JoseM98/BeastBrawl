#pragma once

#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include "../Constants.h"

#include <math.h>
#include <memory>

using namespace std;
using namespace glm;
#define PI 3.141592

class Car;
class Camera;
class CCar;
class CTransformable;
class CExternalForce;
class CCamera;
class CSpeed;
class CNitro;

class Physics {
   public:
    Physics();
    ~Physics(){};
    void update(Car *car, Camera *cam);
    void Accelerate(Car *);
    void TurnLeft(Car *, Camera *);
    void TurnRight(Car *, Camera *);
    void Turn(Car *, Camera *, bool right);

    void NotAcceleratingOrDecelerating(Car *);
    void Decelerate(Car *);
    void NotTurning(Car *, Camera *);

    void UpdateHuman(Car *car);
    // void AccelerateHuman(CCar &cCar, CNitro &cNitro) const;
    void TurnLeftHuman(CCar &cCar) const;
    void TurnRightHuman(CCar &cCar) const;
    // void NotAcceleratingOrDeceleratingHuman(CCar &cCar, CNitro &cNitro) const;
    // void DecelerateHuman(CCar &cCar, CNitro &cNitro) const;
    void NotTurningHuman(CCar &cCar) const;
    void RepositionWheelInCenter(CCar *cCar, CCamera *cCamera);

    void UpdateEveryFrame(Car *car, Camera *cam, const double percentTick);

   protected:
   private:
    void CalculatePosition(CCar *cCar, CTransformable *cTransformable, CSpeed *cSpeed, CExternalForce *cExternalForce);
    void CalculatePositionReverse(CCar *cCar, CTransformable *cTransformable, CExternalForce *cExternalForce);
    void CalculatePositionCamera(CTransformable *cTransformableCar, CTransformable *cTransformableCamera, CCamera *cCamera);
    glm::vec3 CalculateVecDirCar(CTransformable *cTransformable) const;
    void FrictionExternalForce(CCar *cCar, CExternalForce *externalForce) const;
    void CalculateWheelRotationWhenNotTurning(CCar *cCar) const;
};
#pragma once

#include <memory>
#include <vector>
#include "../../include/glm/vec3.hpp"

using namespace std;

class Manager;
class Entity;
class CTransformable;
class CBoundingSphere;
class CBoundingPlane;
class CBoundingOBB;
class CExternalForce;
class IntersectData;
class CCar;


class CLPhysics {
   public:
    CLPhysics();
    ~CLPhysics(){};

    void AddManager(Manager &e);
    void Simulate(float delta);
    void Update(float delta);
    void HandleCollisions();
    void HandleCollisionsWithPlanes();
    void HandleCollisionsWithOBB();

    IntersectData HandleCollisionsRayWithSpheres(CTransformable &trCar1, CTransformable &trCar2, CBoundingSphere &spCar2, const glm::vec3 &normalRay);
    IntersectData HandleCollisionsRayWithPlane(CTransformable &trRayOrigin,  glm::vec3 &rayNormalNormalized, CBoundingPlane &planeObject);
    // void Integrate(Entity &e, float delta);

    static void RunTests();

   protected:
   private:
    bool HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccar2, CExternalForce &cExtForc1, CExternalForce &cExtForc2);
    void HandleCollisions(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccar1, bool mainCar, CBoundingPlane &plane);
    void HandleCollisions(CTransformable &trCar, CBoundingSphere &spCar, CCar &ccarCar, bool mainCar, CBoundingOBB &obb, CExternalForce &cExternalForce);
    void PositionSphereIntoTransformable(CTransformable &tr, CBoundingSphere &sp);
    void SeparateSpheres(CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1,
                                 CTransformable &trCar2, CBoundingSphere &spCar2, CCar &ccarCar2);
    void SeparateSphereFromPlane(IntersectData &intersData, CTransformable &trCar1, CBoundingSphere &spCar1, CCar &ccarCar1, CBoundingPlane &plane) const;
    void ReflectCollision(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2);
    void ReflectSpherePlane(CTransformable &trCar, CCar &cCar1, CBoundingPlane &plane);
    void ExchangeVectors(CTransformable &trCar1, CCar &cCar1, CTransformable &trCar2, CCar &cCar2);
    void SonarChoque(bool mainCar);
    void checkCollisionNitro(Entity* car1, Entity* car2);
    double Angle2Vectors(const glm::vec3 &a, const glm::vec3 &b) const;
    glm::vec3 CalculateProyectPointRecta(const glm::vec3 &extrem1, const glm::vec3 &extrem2, const glm::vec3 &point_) const;
    glm::vec3 CalculateVecDirCar(CTransformable &cTransformable) const;
    vector<Manager *> managers;
};
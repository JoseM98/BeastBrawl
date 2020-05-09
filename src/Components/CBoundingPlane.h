#pragma once

#include "Component.h"

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <CLPhysics/IntersectData.h>

using namespace glm;

class CBoundingSphere;
struct CCar;
struct CTransformable;

class CBoundingPlane : public Component {
   public:
    CBoundingPlane(const vec3 &a_, const vec3 &b_, const vec3 &c_, const vec3 &d_);

    // void Normalize();
    IntersectData IntersectSphere(const CBoundingSphere &);
    IntersectData IntersectSphereCenter(const CBoundingSphere &); 

    IntersectData IntersectPlane(const CBoundingPlane &other) const;

    IntersectData IntersectRay(const glm::vec3 &posRayOrigin, const glm::vec3 &rayNormalNormalized) const;
    IntersectData IntersectRay2(const glm::vec3 &posRayOrigin, const glm::vec3 &rayNormalNormalized) const;
    
    void intersectLineSphere(const vec3 &vecLine,const vec3 &point, const float &radius, vec3 *retrunM, vec3 *returnN) const;

    // calculo de la proyeccion de un punto sobre el plano
    vec3 IntersectPoint(const vec3 &point) const;
    // Teniendo un punto en el plano, comprobamos si se encuentra entre sus 4 vertices
    bool membershipPoint(const vec3 &point) const;

    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
    glm::vec3 d;


    vec4 equationPlane; // ecuacion general del plano
    //ejemplo: 2x -y + 3z + 4 = 0
    const glm::vec3 normal;
    const glm::vec3 normalizedNormal;
    const float distance;

    private:
    // calculo de la ecuacion general de un plano mediante 3 puntos
    vec4 equationPlane3Points(const vec3 &a, const vec3 &b, const vec3 &c) const;
    double Angle2Vectors(const vec3 &a, const vec3 &b) const;
    glm::vec3 CalculateVecDirCar(const CTransformable &cTransformable) const;
};
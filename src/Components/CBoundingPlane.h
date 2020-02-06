#pragma once

#include "../../include/glm/geometric.hpp"
#include "../../include/glm/vec3.hpp"
#include "../../include/glm/vec4.hpp"
#include "../CLPhysics/IntersectData.h"

#include "../Components/CTransformable.h"
#include "../Components/CCar.h"

#include "Component.h"

using namespace glm;

class CBoundingSphere;
struct CCar;
struct CTransformable;

class CBoundingPlane : public Component {
   public:
    CBoundingPlane(const vec3 &a_, const vec3 &b_, const vec3 &c_, const vec3 &d_);

    // void Normalize();
    IntersectData IntersectSphere(const CBoundingSphere &other, const CTransformable &trCar, const CCar &ccarCar);

    IntersectData IntersectPlane(const CBoundingPlane &other) const;

    IntersectData IntersectRay(const glm::vec3 &posRayOrigin, const glm::vec3 &rayNormalNormalized) const;

    void intersectLineSphere(const vec3 &vecLine,const vec3 &point, const float &radius, vec3 *retrunM, vec3 *returnN) const;

    // calculo de la proyeccion de un punto sobre el plano
    vec3 IntersectPoint(const vec3 &point) const;
    // Teniendo un punto en el plano, comprobamos si se encuentra entre sus 4 vertices
    bool membershipPoint(const vec3 &point) const;

    vec3 a;
    vec3 b;
    vec3 c;
    vec3 d;


    vec4 equationPlane; // ecuacion general del plano
    //ejemplo: 2x -y + 3z + 4 = 0
    const vec3 normal;
    const vec3 normalizedNormal;
    const float distance;

    private:
    // calculo de la ecuacion general de un plano mediante 3 puntos
    vec4 equationPlane3Points(const vec3 &a, const vec3 &b, const vec3 &c) const;
    double Angle2Vectors(const vec3 &a, const vec3 &b) const;
    glm::vec3 CalculateVecDirCar(const CTransformable &cTransformable) const;
};
#pragma once

#include <glm/glm.hpp>
#include "../CLPhysics/IntersectData.h"
#include "Component.h"

using namespace glm;

struct CTransformable;
struct CBoundingSphere;

class CBoundingCilindre : public Component {
   public:
    CBoundingCilindre(const glm::vec3 &extreme1_, const glm::vec3 &extreme2_, float _radius);

    [[nodiscard]] IntersectData IntersectSphere(const CBoundingSphere &other) const;

    [[nodiscard]] IntersectData IntersectRay(const CTransformable &transCarRay, const glm::vec3 &normalRay) const;

    glm::vec3 extreme1 { glm::vec3(0.0f,0.0f,0.0f)};
    glm::vec3 extreme2 { glm::vec3(0.0f,0.0f,0.0f)};
    glm::vec3 centerMass { glm::vec3(0.0f,0.0f,0.0f)};
    const float radius{ DEFAULT_SPHERE_RADIUS };
    static constexpr float DEFAULT_SPHERE_RADIUS { 10.f };


   private:

    void CalculateCenterMass(const glm::vec3 &ext1, const glm::vec3&ext2);
    [[nodiscard]] glm::vec3 CalculateProyectPointOnRecta(const glm::vec3 &point_) const;
    [[nodiscard]] double EuclideanDis(const glm::vec3 &p1, const glm::vec3 &p2) const;

};
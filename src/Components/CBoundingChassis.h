#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include "Component.h"



using namespace glm;
using namespace std;

class CBoundingSphere;
class IntersectData;
struct CCar;
struct CTransformable;
struct CBoundingPlane;
struct CBoundingCilindre;

class CBoundingChassis : public Component {
   public:
    CBoundingChassis(const vec3 &spCenterBehind, const float &spRadiusBehind, const vec3 &spCenterFront, const float &spRadiusFront);

    shared_ptr<CBoundingSphere> sphereBehind;
    shared_ptr<CBoundingSphere> sphereFront;
    shared_ptr<CBoundingCilindre> cilindre;
};
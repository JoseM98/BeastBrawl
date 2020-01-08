#include "BoundingWall.h"
#include <iostream>
#include "../Components/CBoundingPlane.h"
#include "../Components/CId.h"

class Position;
using namespace std;

BoundingWall::BoundingWall() : BoundingWall(vec3(-20.f, 20.f, /*-460.f*/ 200.f)) {
    // shared_ptr<CId> cId = make_shared<CId>();
    // // TODO: estos datos son de pegote, cambiar
    // vec3 pos(-20.f, 20.f, -460.f);
    // vec3 a(pos.x, pos.y + 20.f, pos.z);
    // vec3 b(pos.x + 20.f, pos.y + 20.f, pos.z);
    // vec3 c(pos.x + 20.f, pos.y, pos.z);
    // vec3 d(pos.x, pos.y, pos.z);
    // shared_ptr<CBoundingPlane> cPlane = make_shared<CBoundingPlane>(a, b, c, d);
    // AddComponent(cId);
    // AddComponent(cPlane);
}

BoundingWall::BoundingWall(const vec3 &pos) : BoundingWall(pos, true, true) {
}

BoundingWall::BoundingWall(const vec3 &pos, bool orientation1, bool orientation2) {
    shared_ptr<CId> cId = make_shared<CId>();
    vec3 a(pos.x, pos.y + 20.f, pos.z);
    vec3 b;
    vec3 c;
    vec3 d(pos.x, pos.y, pos.z);
    if (orientation1) {
        b = vec3(pos.x + 20.f, pos.y + 20.f, pos.z);
        c = vec3(pos.x + 20.f, pos.y, pos.z);
    } else {
        b = vec3(pos.x, pos.y + 20.f, pos.z + 20.f);
        c = vec3(pos.x, pos.y, pos.z + 20.f);
    }
    shared_ptr<CBoundingPlane> cPlane;
    if (orientation2)
        cPlane = make_shared<CBoundingPlane>(a, b, c, d);
    else
        cPlane = make_shared<CBoundingPlane>(d, c, b, a);
    AddComponent(cId);
    AddComponent(cPlane);
}

BoundingWall::BoundingWall(const vec3 &a_, const vec3 &b_, const vec3 &c_, const vec3 &d_) {
    shared_ptr<CId> cId = make_shared<CId>();
    shared_ptr<CBoundingPlane> cPlane = make_shared<CBoundingPlane>(a_, b_, c_, d_);
    AddComponent(cId);
    AddComponent(cPlane);
}

BoundingWall::~BoundingWall() {
}

#pragma once

#include <stdint.h>

enum CompType {
    LastPositionComp,
    SpeedComp,
    IdComp,
    TypeComp,
    TextureComp,
    MeshComp,
    TransformableComp,
    CameraComp,
    WheelComp,
    WayPointComp,
    PosDestination,
    CarComp,
    PowerUpComp,
    BoxPowerUpComp,
    ShieldComp,
    NitroComp,
    RoboJoroboComp,
    TotemComp,
    TargetEntityComp,
    WayPointEdgesComp,
    DimensionsComp,
    CompBoundingSphere,
    CompBoundingAABox,
    CompExternalForce,
    CompBoundingPlane,
    CompBoundingClindre,
    CompBoundingOBB,
    CompBoundingChassis,
    CompBoundingRay,
    CollidingComp,
    PathComp,
    NavMeshComp,
    CurrentNavMeshComp,
    TargetNavMeshComp,
    CompRay,
    NamePlateComp,
    MovementComp
};

class Component {
   public:
    Component(){};
    ~Component(){};

    CompType getType() { return m_compType; };
    static const uint32_t ID_DIFFERENCE{100000};
    static uint32_t nextId;
    uint32_t ID{nextId++};

   protected:
    CompType m_compType;

   private:
};

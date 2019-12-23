#pragma once

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
    PathComp
};

class Component
{
public:
    Component() {};
    ~Component() {};

    CompType getType() { return m_compType; };

protected:
    CompType m_compType;

private:
    
};

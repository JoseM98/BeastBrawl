#include "Car.h"
#include "../Components/CSpeed.h"
#include "../Components/CId.h"
#include "../Components/CType.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"
#include "../Components/CTransformable.h"
#include "../Components/CCar.h"
#include <iostream>

class Position;
using namespace std;

Car::Car(){
    // default values
    glm::vec3 pos   = glm::vec3(-20.0f, 20.0f, -300.0f);
    glm::vec3 rot   = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scale = glm::vec3(0.6f, 0.6f, 0.6f);
    string texture = "";
    string mesh    = "kart.obj";
    float maxSpeed = 20.0, acceleration = .15, friction = 0.1, slowDown = 0.25;
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::AnimatedMesh);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(maxSpeed, acceleration, friction, slowDown);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);
    cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;
}

Car::Car(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
    string texture, string mesh, 
    float maxSpeed, float acceleration , float carFriction, float carSlowDown) 
{

    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(pos, rot, scale); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CCar> cCar = make_shared<CCar>(maxSpeed, acceleration, carFriction, carSlowDown);
    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cCar);
    cout << "Acabamos de llamar al constructor default de car, su transformable es " << cTransformable << endl;


}



Car::Car(glm::vec3 _position) 
    : Car()
{
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position.x = _position.x;
    cTransformable->position.y = _position.y;
    cTransformable->position.z = _position.z;
}



Car::~Car()
{
    
}
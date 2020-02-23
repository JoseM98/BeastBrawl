#include "BoxPowerUp.h"

#include <Components/CId.h>
#include <Components/CType.h>
#include <Components/CTexture.h>
#include <Components/CMesh.h>
#include <Components/CTransformable.h>
#include <Components/CBoxPowerUp.h>
#include "../Components/CRemovableObject.h"

#include <iostream>

class Position;
using namespace std;

BoxPowerUp::BoxPowerUp(){
    // default values
    string texture = "t351sml.jpg";
    string mesh    = "media/ninja.b3d";
    //float maxSpeed = 20.0, acceleration = .15, friction = 0.1, slowDown = 0.25;
    
    shared_ptr<CId> cId   = make_shared<CId>();
    shared_ptr<CType> cType = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformable = make_shared<CTransformable>(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.0f,1.0f,1.0f)); 
    shared_ptr<CTexture> cTexture = make_shared<CTexture>(texture);
    shared_ptr<CMesh> cMesh   = make_shared<CMesh>(mesh);
    shared_ptr<CBoxPowerUp> cBoxPowerUp   = make_shared<CBoxPowerUp>();
    shared_ptr<CRemovableObject> cRemovableObject = make_shared<CRemovableObject>();
    

    AddComponent(cId);
    AddComponent(cType);
    AddComponent(cTransformable);
    AddComponent(cTexture);
    AddComponent(cMesh);
    AddComponent(cBoxPowerUp);
    AddComponent(cRemovableObject); // componente para eliminar la entidad al final y no en medio de la ejecucion
    //AddComponent(cCar);



    //cout << "Acabamos de llamar al constructor default de powerup, su transformable es " << cTransformable << endl;

}

BoxPowerUp::BoxPowerUp(glm::vec3 _position) 
    : BoxPowerUp(){
    CTransformable *cTransformable = (CTransformable *)m_components[CompType::TransformableComp].get();
    cTransformable->position.x = _position.x;
    cTransformable->position.y = _position.y;
    cTransformable->position.z = _position.z;

    

    //typePowerUp = _typePowerUp;

    //cout << "Creamos un powerUp del tipo:  " << typePowerUp << endl;
}



BoxPowerUp::~BoxPowerUp(){

}

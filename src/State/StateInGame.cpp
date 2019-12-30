#include "StateInGame.h"
#include <chrono>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <limits.h>
#include <algorithm>

#include "../Components/CTransformable.h"
#include "../Components/CWayPointEdges.h"
#include "../Components/CTotem.h"
#include "../Components/CPath.h"
#include "../CLPhysics/CLPhysics.h"
#include "../Managers/Manager.h"
#include "../Components/CTexture.h"
#include "../Components/CMesh.h"

typedef std::chrono::high_resolution_clock Clock;

using namespace std;
using namespace chrono;


StateInGame::StateInGame() {
    
    physics = make_unique<Physics>(deltaTime);

    eventManager = EventManager::GetInstance();

    manPowerUps = make_shared<ManPowerUp>();
    phisicsPowerUp = make_shared<PhysicsPowerUp>();
    manBoxPowerUps = make_shared<ManBoxPowerUp>();
    manTotems = make_shared<ManTotem>();
    ground = make_shared<GameObject>(glm::vec3(10.0f, -0.5f, 150.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.5f, 1.5f, 1.5f), "", "training_ground.obj");
    cam = make_shared<Camera>(glm::vec3(100.0f, 600.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    manWayPoint = make_shared<ManWayPoint>(); //Se crean todos los waypoints y edges

    auto cWayPoint = static_cast<CWayPoint*>(manWayPoint->GetEntities()[2]->GetComponent(CompType::WayPointComp).get());


    manCars = make_shared<ManCar>(physics.get(), cam.get());
    //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars->CreateCarAI(glm::vec3(50.0f, 20.0f, 100.0f), cWayPoint);
    stack<int> pathInit;
    pathInit.push(3);
    pathInit.push(1);
    pathInit.push(2);
    manCars->GetEntitiesAI()[0]->SetPath(pathInit);


   // auto cActualPowerUp = static_cast<CPowerUp*>(manCars->GetEntitiesAI()[0]->GetComponent(CompType::PowerUpComp).get());
   // cActualPowerUp->typePowerUp = typeCPowerUp::MelonMolon;
    //auto cPath = static_cast<CPath*>(manCars->GetEntitiesAI()[0]->GetComponent(CompType::PathComp).get());
   // auto cActualPowerUp = static_cast<CPowerUp*>(manCars->GetEntitiesAI()[0]->GetComponent(CompType::PowerUpComp).get());
   // cActualPowerUp->typePowerUp = typeCPowerUp::TeleBanana;
    // while(!cPath->stackPath.empty()){
    //     auto node = cPath->stackPath.top();
    //     cPath->stackPath.pop();
    //     cout << node << " - ";
    // }

    auto cWayPointAI2 = static_cast<CWayPoint*>(manWayPoint->GetEntities()[1]->GetComponent(CompType::WayPointComp).get());
   //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars->CreateCarAI(glm::vec3(70.0f, 20.0f, 20.0f), cWayPointAI2);
    stack<int> pathInit2;
    pathInit2.push(4);
    pathInit2.push(0);
    pathInit2.push(2);
    manCars->GetEntitiesAI()[1]->SetPath(pathInit2);


    auto cWayPointAI3 = static_cast<CWayPoint*>(manWayPoint->GetEntities()[0]->GetComponent(CompType::WayPointComp).get());
   //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars->CreateCarAI(glm::vec3(0.0f, 20.0f, -40.0f), cWayPointAI3);
    stack<int> pathInit3;
    pathInit3.push(5);
    pathInit3.push(0);
    pathInit3.push(5);
    manCars->GetEntitiesAI()[2]->SetPath(pathInit3);



    // Inicializamos las facadas
    renderFacadeManager = RenderFacadeManager::GetInstance();
    //renderFacadeManager->InitializeIrrlicht();

    inputFacadeManager = InputFacadeManager::GetInstance();
    //inputFacadeManager->InitializeIrrlicht();

    physicsFacadeManager = PhysicsFacadeManager::GetInstance();
    //physicsFacadeManager->InitializeIrrlicht();

    //Almacenamos los motores
    renderEngine = renderFacadeManager->GetRenderFacade();
    renderEngine->FacadeSuscribeEvents();
    renderEngine->FacadeInitHUD();
    inputEngine = inputFacadeManager->GetInputFacade();
    physicsEngine = physicsFacadeManager->GetPhysicsFacade();

    // Creamos sistemas
    physicsAI = make_shared<PhysicsAI>();
    collisions = make_shared<Collisions>();
    sysBoxPowerUp = make_shared<SystemBoxPowerUp>();


    // Entidades iniciales
    renderEngine->FacadeAddObjectCar(manCars.get()->GetCar().get());  //Anyadimos el coche
    for (shared_ptr<Entity> carAI : manCars->GetEntitiesAI())         // Anyadimos los coche IA
        renderEngine->FacadeAddObject(carAI.get());
    renderEngine->FacadeAddObject(ground.get());  //Anyadimos el suelo

    for (shared_ptr<WayPoint> way : manWayPoint->GetEntities()) {
        auto components = way->GetComponents();
        auto mapWaypoint = components.find(CompType::WayPointComp);
        auto cWayPoint = static_cast<CWayPoint*>(mapWaypoint->second.get());
        // solo debemos crear las Box si el type del waypoint es "1"
        if(cWayPoint->type == 1){
            manBoxPowerUps->CreateBoxPowerUp(glm::vec3(cWayPoint->position));
        }
    }

    //Añadimos todos los power ups
    for (shared_ptr<Entity> bpu : manBoxPowerUps->GetEntities())
        renderEngine->FacadeAddObject(bpu.get());
        
    renderEngine->FacadeAddCamera(cam.get());

    //lastFPS = -1;
    // CREAMOS EL TOTEM
    manTotems->CreateTotem(glm::vec3(-100.0,20.0,-100.0));
    renderEngine->FacadeAddObject(manTotems->GetEntities()[0].get());

// --------------------------------------------------------------------------------------------------------------------------------------------
    totemOnCar = make_shared<Entity>();
    glm::vec3 postoTemOnCar   = glm::vec3(40.0f, -100.0f, 30.0f);
    glm::vec3 rotTotemOnCar   = glm::vec3(0.0f, 90.0f, 0.0f);
    glm::vec3 scaleTotemOnCar = glm::vec3(0.5f, 0.5f, 0.5f);
    shared_ptr<CId> cIdTotemOnCar   = make_shared<CId>();
    shared_ptr<CType> cTypeTotemOnCar = make_shared<CType>(ModelType::Cube);
    shared_ptr<CTransformable> cTransformableTotemOnCar = make_shared<CTransformable>(postoTemOnCar, rotTotemOnCar, scaleTotemOnCar); 
    totemOnCar->AddComponent(cIdTotemOnCar);
    totemOnCar->AddComponent(cTypeTotemOnCar);
    totemOnCar->AddComponent(cTransformableTotemOnCar);
    totemOnCar->AddComponent(make_shared<CTexture>("particlegreen.jpg"));
    totemOnCar->AddComponent(make_shared<CMesh>("media/ninja.b3d"));
    renderEngine->FacadeAddObject(totemOnCar.get());
// ------------------------------------------------------------------------------------------------------------------------------------------------
    //then = renderEngine->FacadeGetTime();
    //then = system_clock::now();

    //inicializamos las reglas del cocheIA de velocidad/aceleracion
    //FuzzyLogic flVelocity;
    physicsAI->InitPhysicsIA(manCars->GetEntitiesAI()[0].get());  // To-Do: hacer que se le pasen todos los coches IA
    cout << "después de init physics ai" << endl;

    // BehaivourTree
    systemBtPowerUp = make_shared<SystemBtPowerUp>();
    systemBtMoveTo  = make_shared<SystemBtMoveTo>(); 
    
    
    clPhysics = make_unique<CLPhysics>();
    clPhysics->AddManager(*manCars.get());
}

StateInGame::~StateInGame() {
    // destructor
}

//Carga los bancos de sonido InGame.
void StateInGame::InitState() {
    soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    if (soundEngine){
        soundEngine->SetState(4);
    }
}

void StateInGame::Input() {
    renderEngine->FacadeCheckInput();
}

void StateInGame::Update() {
    eventManager->Update();


// --------------------------------------------------------------------------------------------------------------------------------------------------------
    bool todosFalse = true;
    auto cTransformTotem = static_cast<CTransformable*>(totemOnCar.get()->GetComponent(CompType::TransformableComp).get());
    cTransformTotem->rotation.y += 0.1;
    for (shared_ptr<Entity> carAI : manCars->GetEntitiesAI()){  // actualizamos los coche IA
        // comprobamos el componente totem y si lo tienen se lo ponemos justo encima para que se sepa quien lo lleva
        auto cTotem = static_cast<CTotem*>(carAI.get()->GetComponent(CompType::TotemComp).get());
        if(cTotem->active){ 
            todosFalse = false;
            auto cTransformCar = static_cast<CTransformable*>(carAI.get()->GetComponent(CompType::TransformableComp).get()); 
            cTransformTotem->position.x = cTransformCar->position.x;
            cTransformTotem->position.z = cTransformCar->position.z;
            cTransformTotem->position.y = 32.0f;
            // supuestamente esta el drawAll que te lo hace no?????????????????
            // si esta cambiando pero no se esta redibujando
        }

    }
    if(todosFalse){
        auto cTotem = static_cast<CTotem*>(manCars.get()->GetCar().get()->GetComponent(CompType::TotemComp).get());
        if(cTotem->active){  
            todosFalse = false; 
            auto cTransformCar = static_cast<CTransformable*>(manCars.get()->GetCar().get()->GetComponent(CompType::TransformableComp).get()); 
            cTransformTotem->position.x = cTransformCar->position.x;
            cTransformTotem->position.z = cTransformCar->position.z;
            cTransformTotem->position.y = 32.0f;
        }
    }
    if(todosFalse)
        cTransformTotem->position.y = -100.0f;

    renderEngine->UpdateTransformable(totemOnCar.get());
// ----------------------------------------------------------------------------------------------------------------------------------------------------------

    // actualizamos el deltatime
    //time_point<system_clock> now = system_clock::now();
    //int64_t milis = duration_cast<milliseconds>(now - then).count();
    //const uint32_t now = renderEngine->FacadeGetTime();

    // con media
    //float currentDelta = (float)(milis) / 100.0;
    //*deltaTime.get() = CalculateDelta(currentDelta);

    // sin media
    // *deltaTime.get() = (float)(milis) / 100.0;

    // BEHAIVOUR TREE
    for(auto actualAI : manCars->GetEntitiesAI()){
        //std::cout << "hola caracolaaaaaaaaaaaaaaaaa2939393399494839839275087346734790393970" << std::endl;
        systemBtMoveTo->update(actualAI.get(), manCars.get(), manPowerUps.get(), manBoxPowerUps.get(), manTotems.get(), manWayPoint.get());

        systemBtPowerUp->update(actualAI.get(), manCars.get(), manPowerUps.get(), manBoxPowerUps.get(), manTotems.get(), manWayPoint.get());
    }
   
    // ACTUALIZACION DE LOS MANAGERS DE LOS COCHES
    manCars->UpdateCar();
    //manCars->UpdateCarAI();
    for(auto actualAI : manCars->GetEntitiesAI()){
        manCars->UpdateCarAI(actualAI.get(),manWayPoint.get());
    }

    // ACTUALIZACION DE LAS FISICAS DE LOS COCHES
    physics->update(manCars->GetCar().get(), cam.get());
    for(auto actualAI : manCars->GetEntitiesAI()){
        physicsAI->Update(actualAI.get(), deltaTime);
    }

    clPhysics->Update(0.1666f);
    sysBoxPowerUp->update(manBoxPowerUps.get());
    phisicsPowerUp->update(manPowerUps->GetEntities());


    // COLISIONES entre BoxPowerUp y player
    collisions->IntersectPlayerBoxPowerUp(manCars.get()->GetCar().get(), manBoxPowerUps.get());
    // COLISIONES entre BoxPowerUp y IA                                                            
    collisions->IntersectCarsBoxPowerUp(manCars.get(), manBoxPowerUps.get());
    // COLISIONES entre powerUp y player
    collisions->IntersectPlayerPowerUps(manCars.get()->GetCar().get(), manPowerUps.get());
    // COLISIONES entre powerUp y IA
    collisions->IntersectsCarsPowerUps(manCars.get(), manPowerUps.get());
    // COLISIONES entre el Player y el Totem
    collisions->IntersectPlayerTotem(manCars.get()->GetCar().get(), manTotems.get());
    // COLISIONES  entre la IA y el Totem
    collisions->IntersectCarsTotem(manCars.get(), manTotems.get());


    // Actualizaciones en Irrlich
    renderEngine->UpdateCamera(cam.get());
    physicsEngine->UpdateCar(manCars.get()->GetCar().get(), cam.get());
    for (shared_ptr<Entity> carAI : manCars->GetEntitiesAI())  // actualizamos los coche IA
        physicsEngine->UpdateCarAI(carAI.get());

    for (shared_ptr<Entity> actualPowerUp : manPowerUps->GetEntities())  // actualizamos los powerUp en irrlich
        physicsEngine->UpdatePowerUps(actualPowerUp.get());

}



void StateInGame::Render() {
    auto carAI = manCars->GetEntitiesAI()[0].get();
    bool isColliding = collisions->Intersects(manCars.get()->GetCar().get(), carAI);

    renderEngine->FacadeBeginScene();
    // renderEngine->FacadeDraw();  //Para dibujar primitivas debe ir entre el drawAll y el endScene
    renderEngine->FacadeDrawAll();
    renderEngine->FacadeDrawHUD(manCars->GetCar().get(), manCars.get());
    renderEngine->FacadeDrawGraphEdges(manWayPoint.get());
    renderEngine->FacadeDrawBoundingBox(manCars.get()->GetCar().get(), isColliding);
    
    
    for(auto actualAI : manCars->GetEntitiesAI()){
       renderEngine->FacadeDrawBoundingBox(actualAI.get(), false); 
    }
    renderEngine->FacadeDrawBoundingBox(carAI, isColliding);

    for(auto actualPowerUp : manPowerUps->GetEntities())
        renderEngine->FacadeDrawBoundingBox(actualPowerUp.get(), false);

    renderEngine->FacadeEndScene();
}

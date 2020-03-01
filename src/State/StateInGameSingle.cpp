#include "StateInGameSingle.h"

#include "../Systems/Utils.h"

StateInGameSingle::StateInGameSingle() : StateInGame() {
    systemBtPowerUp = make_unique<SystemBtPowerUp>();
    systemBtMoveTo = make_unique<SystemBtMoveTo>();
    systemBtLoDMove = make_unique<SystemBtLoDMove>();
    systemPathPlanning = make_unique<SystemPathPlanning>();

    InitVirtualMethods();

    //std::cout << "ENTRAMOS AL MANAGER DE NAVMESH LOCOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << std::endl;
    CAMBIARCosasNavMesh(*manCars, *manNavMesh);
    //std::cout << "despues de la llamada LOOOOOOOOOOOCOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << std::endl;
}

StateInGameSingle::~StateInGameSingle() = default;

void StateInGameSingle::InitState() {
    StateInGame::InitState();
}

void StateInGameSingle::Input() {
    renderEngine->FacadeCheckInputSingle();
    inputEngine->CheckInput();
}

void StateInGameSingle::Update() {
    //auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(start);
    //auto value = now_ms.time_since_epoch();
    //long duration = value.count() - (timeElapsed - updateTickTime);
    //std::cout << "valor: " << duration << std::endl;
    //std::chrono::microseconds dur(duration);
    //std::chrono::time_point<std::chrono::system_clock> dt(dur);
    //start = dt;
    //float prueba = duration_cast<std::chrono::microseconds>(system_clock::now()-startTime).count();
    //std::cout << "valor: " << value.count() << "     -- otos valores: " << timeElapsed - updateTickTime << std::endl;

    //std::cout << system_clock::now() <<
    //setDeltaTime(timeElapsed/1000000.0);
    //physics->setDeltaTime(timeElapsed/1000000.0);
    //std::cout << "Tiempo: " << timeElapsed/1000000.0 << std::endl;  // esto es mi deltaTime

    // actualizar cosas normales

    StateInGame::Update();
    for (const auto& actualAI : manCars->GetEntities()) {  // CUIDADO!!! -> el static cast que solo se use en el single player, si no peta
        if (static_cast<Car *>(actualAI.get())->GetTypeCar() == TypeCar::CarAI) {
            manCars->UpdateCarAI(
                static_cast<CarAI *>(actualAI.get()),
                manPowerUps.get(),
                manBoxPowerUps.get(),
                manTotems.get(),
                manWayPoint.get(),
                manNavMesh.get(),
                manBoundingWall.get(),
                systemBtPowerUp.get(),
                systemBtMoveTo.get(),
                systemBtLoDMove.get(),
                systemPathPlanning.get());
        }
    }
    CAMBIARPositionTotemAboveCar();

    // COLISIONES entre powerUp y IA
    //collisions->IntersectsCarsPowerUps(manCars.get(), manPowerUps.get(), manNavMesh.get());
    // COLISIONES entre BoxPowerUp y IA
    //collisions->IntersectCarsBoxPowerUp(manCars.get(), manBoxPowerUps.get());
    // COLISIONES  entre la IA y el Totem
    //collisions->IntersectCarsTotem(manCars.get(), manTotems.get());

    // Actualizamos posicion en Irrlicht
    for (const auto& actualAI : manCars->GetEntities()) {  // CUIDADO!!! -> el static cast que solo se use en el single player, si no peta
        if (static_cast<Car *>(actualAI.get())->GetTypeCar() == TypeCar::CarAI) {
            physicsEngine->UpdateCarAI(actualAI.get());
        }
    }
}

void StateInGameSingle::Render(double timeElapsed) {
    double percentTick = std::min(1.0, (timeElapsed / Constants::TIME_BETWEEN_UPDATES_us));
    // cout << "PercentTick[" << percentTick << "]" << endl;
    physics->UpdateEveryFrame(manCars->GetCar().get(), cam.get(), percentTick);
    for (const auto &carEnt : manCars->GetEntities()) {
        const auto car = static_cast<Car *>(carEnt.get());
        physics->UpdateEveryFrame(car, percentTick);
    }

    for (const auto& cars : manCars->GetEntities()) {
        renderEngine->FacadeDrawBoundingBox(cars.get(), false);
    }
    //renderEngine->FacadeDrawBoundingBox(carPrincial, isColliding);
    // const auto &car = manCars->GetCar();
    // const auto cCar = static_cast<CCar *>(car.get()->GetComponent(CompType::CarComp).get());
    // const auto cTransformable = static_cast<CTransformable *>(car.get()->GetComponent(CompType::TransformableComp).get());

    // double currentSpeed = cCar->speed * Constants::DELTA_TIME;
    // cout << Utils::getISOCurrentTimestampMillis() << " El percentTick es " << percentTick << " y la current speed es " << currentSpeed
    //      << "\n     la pos actual es " << cTransformable->positionPrev.x << "," << cTransformable->positionPrev.y << "," << cTransformable->positionPrev.z
    //      << "\n     la pos siguiente es " << cTransformable->positionNext.x << "," << cTransformable->positionNext.y << "," << cTransformable->positionNext.z
    //      << "\n     la pos actual es " << cTransformable->position.x << "," << cTransformable->position.y << "," << cTransformable->position.z
    //      << endl;
    StateInGame::Render(timeElapsed);
}

void StateInGameSingle::InitializeCLPhysics(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeCLPhysics(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameSingle::InitializeManagers(Physics *physics, Camera *cam) {
    StateInGame::InitializeManagers(physics, cam);
    CAMBIARInicializarCarAIS(*manCars, *manWayPoint);
}

void StateInGameSingle::InitializeSystems(ManCar &manCars, ManBoundingWall &manWall, ManBoundingOBB &manOBB, ManBoundingGround &manGround, ManPowerUp &manPowerUp, ManNavMesh &manNavMesh, ManBoxPowerUp &manBoxPowerUp, ManTotem &manTotem) {
    StateInGame::InitializeSystems(manCars, manWall, manOBB, manGround, manPowerUp, manNavMesh, manBoxPowerUp, manTotem);
}

void StateInGameSingle::InitializeFacades() {
    StateInGame::InitializeFacades();
}

void StateInGameSingle::AddElementsToRender() {
    StateInGame::AddElementsToRender();
}

void StateInGameSingle::CAMBIARInicializarCarAIS(ManCar &manCars, ManWayPoint &manWayPoint) {
    /*    
    auto cWayPoint = static_cast<CWayPoint *>(manWayPoint.GetEntities()[0]->GetComponent(CompType::WayPointComp).get());
    //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars.CreateCarAI(glm::vec3(-200.0f, 20.0f, 700.0f), cWayPoint);
    stack<int> pathInit;
    pathInit.push(3);
    pathInit.push(1);
    pathInit.push(2);
    manCars.GetEntitiesAI()[0]->SetPath(pathInit);

    auto cWayPointAI2 = static_cast<CWayPoint *>(manWayPoint.GetEntities()[1]->GetComponent(CompType::WayPointComp).get());
    //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars.CreateCarAI(glm::vec3(400.0f, 20.0f, 20.0f), cWayPointAI2);
    stack<int> pathInit2;
    pathInit2.push(4);
    pathInit2.push(0);
    pathInit2.push(2);
    manCars.GetEntitiesAI()[1]->SetPath(pathInit2);

    auto cWayPointAI3 = static_cast<CWayPoint *>(manWayPoint.GetEntities()[0]->GetComponent(CompType::WayPointComp).get());
    //Le asignamos el waypoint inicial, momentaneo a la IA
    manCars.CreateCarAI(glm::vec3(400.0f, 20.0f, -400.0f), cWayPointAI3);
    stack<int> pathInit3;
    pathInit3.push(5);
    pathInit3.push(0);
    pathInit3.push(5);
    manCars.GetEntitiesAI()[2]->SetPath(pathInit3);
*/
    // debug_create_car
    manCars.CreateCarAI(glm::vec3(-50.0f, -100.0f, 700.0f));
    // manCars.CreateCarAI(glm::vec3(400.0f, 10.0f, -50.0f));
    //manCars.CreateHumanCar(glm::vec3(20.0, 10.0, 20.0));
    // manCars.CreateCarAI(glm::vec3(300.0f, 50.0f, -300.0f));
}
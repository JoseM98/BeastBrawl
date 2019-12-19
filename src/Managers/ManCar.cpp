#include "ManCar.h"
#include <functional>
#include <iostream>
#include "../Entities/Camera.h"
#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Systems/Physics.h"
#include "../Components/CPowerUp.h"
#include "../Components/CShield.h"
#include "../Components/CTotem.h"
#include "../Components/CRoboJorobo.h"
#include "../Components/CNitro.h"
#include "../Facade/Render/RenderFacadeManager.h"
#include "../Game.h"


class Position;
using namespace std;

ManCar::ManCar() {
    SubscribeToEvents();
    CreateMainCar();
    rangeVision = 20;   // rango de vision de 20 grados 
    cout << "Hemos creado un powerup, ahora tenemos " << entities.size() << " powerups" << endl;
}

// TODO: este paso de physics es kk, hay que revisarlo de enviarlo como referencia o algo pero me da error
ManCar::ManCar(Physics *_physics, Camera *_cam) : ManCar() {
    this->physics = _physics;
    cout << "Tenemos en ManCar un physics con dir de memoria " << physics << endl;
    this->cam = _cam;
}

void ManCar::UpdateCar(){
    auto cTotem = static_cast<CTotem*>(GetCar()->GetComponent(CompType::TotemComp).get());
    if(cTotem->active){
        cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();

    }
    if(cTotem->accumulatedTime/1000.0 > cTotem->durationTime/1000.0){
        cout << "Has ganado\n";
        Game::GetInstance()->SetState(State::ENDRACE);
    }

}

void ManCar::UpdateCarAI(){
    for(auto carAI : GetEntitiesAI()){
        auto cTotem = static_cast<CTotem*>(carAI->GetComponent(CompType::TotemComp).get());
        if(cTotem->active){
            cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
            cTotem->timeStart = system_clock::now();
            //cout << "El totem lo tiene: " << cTotem->accumulatedTime/1000.0 << "\n";

        }

        if(cTotem->accumulatedTime/1000.0 > cTotem->durationTime/1000.0){
            cout << "Has ganado\n";
            Game::GetInstance()->SetState(State::ENDRACE);

        }
    }
}

ManCar::~ManCar() {
    cout << "Llamando al destructor de ManCar" << endl;
    CarAIs.clear();
    CarAIs.shrink_to_fit();
}

void ManCar::CreateMainCar() {
    car = make_shared<Car>();
}

void ManCar::CreateCar() {
    shared_ptr<Car> p = make_shared<Car>();
    entities.push_back(p);
}


void ManCar::CreateCarAI(glm::vec3 _position,  CWayPoint* _waypoint) 
{
	shared_ptr<CarAI> p = make_shared<CarAI>(_position);
    CarAIs.push_back(p);
    p->SetWayPoint(_waypoint); // tiene que tener un waypoint inicial To-Do: cambiar esto
}


void ManCar::CreateCarAI() 
{
	shared_ptr<CarAI> p = make_shared<CarAI>();
    CarAIs.push_back(p);
}

void ManCar::SubscribeToEvents() {
    // auto accelerateCar = [&](Data d) {
    //     cout << "Estamos ejecutando lambda " << endl;
    //     /*Car *c = this->GetCar().get();
    //     auto components = c->GetComponents();
    //     CTransformable *trans = static_cast<CTransformable*>(components[CompType::TransformableComp].get()); 
    //     cout << "El coche está en  " << trans->position.x << "," << trans->position.y << "," << trans->position.z << endl;*/
    //     //physics->Accelerate(car.get(), cam);
    // };

    /*int (*accelerate) (Data) = [&](Data d) {
        physics->Accelerate(car.get(), cam);
        return 0;
    };*/
    //int (*func_pointer) (int) = [](int a) { return a; };

    //accelerateCar(0);
    auto lambdaAccelerate = [&](DataMap d){this->AccelerateCar(d);};
    //
    //auto lambdaGuardaAccel = [&lambdaAccelerate](DataMap d) {lambdaAccelerate(d);};


    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_I,
        lambdaAccelerate,
        // [&](DataMap d){this->AccelerateCar(d);},
        "AccelerateCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_O,
        bind(&ManCar::Decelerate, this, placeholders::_1),
        "Decelerate"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_A,
        bind(&ManCar::TurnLeftCar, this, placeholders::_1),
        "TurnLeftCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_D,
        bind(&ManCar::TurnRightCar, this, placeholders::_1),
        "TurnRightCar"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::NO_I_O_PRESS,
        bind(&ManCar::NotAcceleratingOrDecelerating, this, placeholders::_1),
        "NotAcceleratingOrDecelerating"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::NO_A_D_PRESS,
        bind(&ManCar::NotTurning, this, placeholders::_1),
        "NotTurning"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CATCH_BOX_POWERUP,
        bind(&ManCar::CatchPowerUp, this, placeholders::_1),
        "CatchPowerUp"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::CATCH_AI_BOX_POWERUP,
        bind(&ManCar::CatchPowerUpAI, this, placeholders::_1),
        "CatchPowerUpAI"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::PRESS_SPACE,
        bind(&ManCar::ThrowPowerUp, this, placeholders::_1),
        "ThrowPowerUp"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::THROW_POWERUP_AI,
        bind(&ManCar::ThrowPowerUpAI, this, placeholders::_1),
        "ThrowPowerUpAI"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_ENTITY_POWERUP,
        bind(&ManCar::CollisionPowerUp, this, placeholders::_1),
        "CollisionPowerUp"));
    
    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_ENTITY_AI_POWERUP,
        bind(&ManCar::CollisionPowerUpAI, this, placeholders::_1),
        "CollisionPowerUpAI"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_PLAYER_TOTEM,
        bind(&ManCar::CatchTotemPlayer, this, placeholders::_1),
        "CatchTotemPlayer"));

    EventManager::GetInstance()->SuscribeMulti(Listener(
        EventType::COLLISION_AI_TOTEM,
        bind(&ManCar::CatchTotemAI, this, placeholders::_1),
        "CatchTotemAI"));

}



void ManCar::CatchTotemAI(DataMap d){

    std::cout << "Coge el totem una IA -- ERES MUUUY MALO COLEGA!" << std::endl;
    auto cTotem = static_cast<CTotem*>(any_cast<Entity*>(d["actualCar"])->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();

}

void ManCar::CatchTotemPlayer(DataMap d){
    std::cout << "Cogemos el Totem locoooo!" << std::endl;
    auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();

}

void ManCar::UseTotem(Entity* carWinTotem){
    auto cTotem = static_cast<CTotem*>(carWinTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = true;
    cTotem->timeStart = system_clock::now();
    std::cout << "Has utilizado el robo jorobo de p*** madre coleeega ahora es tuyo" << std::endl; 
}

void ManCar::ThrowTotem(Entity* carLoseTotem){
    auto cTotem = static_cast<CTotem*>(carLoseTotem->GetComponent(CompType::TotemComp).get());
    cTotem->active = false;
    cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
    std::cout << "El tiempo acumulado del totem hasta ahora del jugador es de:  " << cTotem->accumulatedTime/1000.0 << std::endl; 
}


bool ManCar::useRoboJorobo(Entity* newCarWithTotem){
    // recorremos las IA
    for(shared_ptr<Entity> carAI : CarAIs){
        auto cTotem = static_cast<CTotem*>(carAI.get()->GetComponent(CompType::TotemComp).get()); 
        // Si algun coche tenia el totem .... lo pierde, comprobamos que no sea el mmismo coche con las ID
        if(cTotem->active == true && newCarWithTotem!=carAI.get()){
            ThrowTotem(carAI.get());
            //al perderlo se lo asignamos al que ha usado el robo jorobo
            UseTotem(newCarWithTotem);

            return true;                                                               // para salirnos y no hacer mas calculos
        }
    }
    // comprobamos el player
    auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get()); 
    if(cTotem->active == true && car.get()!=newCarWithTotem){
        ThrowTotem(car.get());
        //al perderlo se lo asignamos al que ha usado el robo jorobo
        UseTotem(newCarWithTotem);
        return true;                                                               // para salirnos y no hacer mas calculos
    }
    return false;
}



void ManCar::CollisionPowerUp(DataMap d){
    std::cout << "Nos ha dado un powerUp neneeeee!" << std::endl;
    // debemos desactivar el powerUp y para el contador de tiempo del totem
    auto cShield = static_cast<CShield*>(car.get()->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp == false){            // comprobamos si tiene el escudo
        auto cTotem = static_cast<CTotem*>(car.get()->GetComponent(CompType::TotemComp).get());
        if(cTotem->active == true){
            ThrowTotem(car.get());
        }
        // Reducimos la velocidad -- TODO --> no solo reducir la velocidad a 0
        auto cCar = static_cast<CCar*>(car.get()->GetComponent(CompType::CarComp).get());
        cCar->speed = 0.0f;
    }else{
        std::cout << "El escudo me salvo el culito :D" << std::endl;
        cShield->deactivePowerUp(); // desactivamos el escudo
    }
}


void ManCar::CollisionPowerUpAI(DataMap d){
    // debemos desactivar el powerUp y para el contador de tiempo del totem
    auto cShield = static_cast<CShield*>(any_cast<Entity*>(d["carAI"])->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp == false){            // comprobamos si tiene el escudo
        auto cTotem = static_cast<CTotem*>(any_cast<Entity*>(d["carAI"])->GetComponent(CompType::TotemComp).get());
        if(cTotem->active == true){
            ThrowTotem(any_cast<Entity*>(d["carAI"]));
        }
        // Reducimos la velocidad -- TODO --> no solo reducir la velocidad a 0
        auto cCar = static_cast<CCar*>(any_cast<Entity*>(d["carAI"])->GetComponent(CompType::CarComp).get());
        cCar->speed = 0.0f;  // To-Do: no funciona en la IA por que la logica difusa no la hace acelerar
    }else{
        std::cout << "El escudo me salvo el culito :D" << std::endl;
        cShield->deactivePowerUp(); // desactivamos el escudo
    }
}


CTransformable* ManCar::calculateCloserCar(Entity* actualCar){
    
    // Primero metemos al jugador por lo que no hace falta calcularlo luego
    CTransformable* closestCar = nullptr;
    if(actualCar != car.get()){
        closestCar = static_cast<CTransformable*>(car.get()->GetComponent(CompType::TransformableComp).get());
    }else{
        closestCar = static_cast<CTransformable*>(CarAIs[0].get()->GetComponent(CompType::TransformableComp).get());
    }
    auto cTransActualCar = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float vectorX = closestCar->position.x - cTransActualCar->position.x;
    float vectorZ = closestCar->position.z - cTransActualCar->position.z;
    float distanceMimum = sqrt((vectorX*vectorX) + (vectorZ*vectorZ));

    float distanceNext = 0.0;
    float vectorXNext = 0.0;
    float vectorZNext = 0.0;
    // Para CarAI
    for(shared_ptr<Entity> carAI : CarAIs){
        if(actualCar != carAI.get()){
            auto cTransNextCar = static_cast<CTransformable*>(carAI.get()->GetComponent(CompType::TransformableComp).get()); 
            vectorXNext = cTransNextCar->position.x - cTransActualCar->position.x;     
            vectorZNext = cTransNextCar->position.z - cTransActualCar->position.z;
            distanceNext = sqrt((vectorXNext*vectorXNext) + (vectorZNext*vectorZNext));
            std::cout << "Min: " << distanceMimum << "   Next: " << distanceNext << std::endl;
            if(distanceMimum > distanceNext){
                distanceMimum = distanceNext;
                closestCar = cTransNextCar;
            }
        }
    }
    return closestCar;
}



void ManCar::ThrowPowerUp(DataMap d) {
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
    //auto cRoboJorobo = static_cast<CRoboJorobo*>(car->GetComponent(CompType::RoboJoroboComp).get());
    auto cShield = static_cast<CShield*>(car.get()->GetComponent(CompType::ShieldComp).get());
    auto cNitro = static_cast<CNitro*>(car.get()->GetComponent(CompType::NitroComp).get());
    bool robado = false;  
    if(cPowerUpCar->typePowerUp != typeCPowerUp::None){

        switch (cPowerUpCar->typePowerUp){
            case typeCPowerUp::RoboJorobo:
                //cRoboJorobo->activatePowerUp();
                robado = useRoboJorobo(car.get());
                if (!robado)
                    std::cout << "La has cagado, el Totem no lo tenia nadie..." << std::endl; 
                break;
            case typeCPowerUp::EscudoMerluzo:
                cShield->activatePowerUp();
                break;
            case typeCPowerUp::SuperMegaNitro:
                cNitro->activatePowerUp();
                break;
            default:
                shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                DataMap data;
                data["typePowerUp"] = cPowerUpCar->typePowerUp;
                data["posCocheSalida"] = static_cast<CTransformable*>(car.get()->GetComponent(CompType::TransformableComp).get());;
                // To-Do: actualmente solo se pasa el coche mas cercano, calcular aparte si se encuentra en pantalla
                data["posCochePerseguir"] = calculateCloserCar(car.get()); // To-Do: se le mete el coche desde el que sale, deberia ser el que persigue
                eventManager->AddEventMulti(Event{EventType::PowerUp_Create, data});

                break;
        }
        //std::cout << "Lanzamos el power up fiiiuuuuum" << std::endl;
        cPowerUpCar->typePowerUp = typeCPowerUp::None;
    }
}



void ManCar::ThrowPowerUpAI(DataMap d) {
    auto cPowerUpCar = static_cast<CPowerUp*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::PowerUpComp).get());
    //auto cRoboJorobo = static_cast<CRoboJorobo*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::RoboJoroboComp).get());
    auto cShield = static_cast<CShield*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::ShieldComp).get());
    auto cNitro = static_cast<CNitro*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::NitroComp).get());
    bool robado = false; 
    
    if(cPowerUpCar->typePowerUp != typeCPowerUp::None){
        switch (cPowerUpCar->typePowerUp){
            case typeCPowerUp::RoboJorobo:
                //cRoboJorobo->activatePowerUp();
                robado = useRoboJorobo(any_cast<CarAI*>(d["actualCar"]));
                if (!robado)
                    std::cout << "La has cagado, el Totem no lo tenia nadie..." << std::endl; 
                break;
            case typeCPowerUp::EscudoMerluzo:
                cShield->activatePowerUp();
                break;
            case typeCPowerUp::SuperMegaNitro:
                cNitro->activatePowerUp();
                break;
            default:
                shared_ptr<EventManager> eventManager = EventManager::GetInstance();
                DataMap data;
                data["typePowerUp"] = cPowerUpCar->typePowerUp;
                data["posCocheSalida"] = static_cast<CTransformable*>(any_cast<CarAI*>(d["actualCar"])->GetComponent(CompType::TransformableComp).get());;
                // To-Do: actualmente solo se pasa el coche mas cercano, calcular aparte si se encuentra en pantalla
                data["posCochePerseguir"] = calculateCloserCar(any_cast<CarAI*>(d["actualCar"])); // To-Do: se le mete el coche desde el que sale, deberia ser el que persigue
                eventManager->AddEventMulti(Event{EventType::PowerUp_Create, data});

                break;
        }
        //std::cout << "Lanzamos el power up fiiiuuuuum" << std::endl;
        cPowerUpCar->typePowerUp = typeCPowerUp::None;
        DataMap d;
        d["typePowerUp"] = cPowerUpCar->typePowerUp;
        EventManager::GetInstance()->AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, d});
    }
}

/*
int calculateProbabilityPowerUp(int totalPowerUps, std::vector<int> probabilityPU){
    int sum_of_weight = 0;

    int rnd = rand() % 100+1; // 100%
    for(int i=0; i<totalPowerUps; i++) {
      if(rnd < probabilityPU[i])
        return i;
      rnd -= probabilityPU[i];
    }
}
*/

void ManCar::CatchPowerUp(DataMap d) {
    srand(time(NULL));
    int indx = rand() % 100+1;
    if(indx <= 5)                       // 5%
        indx = 1;
    else if(indx > 5 && indx <= 20)     // 15%
        indx = 2;
    else if(indx > 20 && indx <= 45)    // 25%
        indx = 3;
    else if(indx > 45 && indx <= 60)    // 15%
        indx = 4;
    else if(indx > 60 && indx <= 75)    // 15%
        indx = 5;
    else if(indx > 75)                  //  25%
        indx = 6;

    //None,               // 0
    //RoboJorobo,         // 1
    //SuperMegaNitro,     // 2
    //PudinDeFrambuesa,   // 3
    //EscudoMerluzo,      // 4
    //TeleBanana,         // 5
    //MelonMolon          // 6
    //indx = 5;
    auto cPowerUpCar = static_cast<CPowerUp*>(car.get()->GetComponent(CompType::PowerUpComp).get());
    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;
        std::cout << "Mi super powerUp es:   " << (int)cPowerUpCar->typePowerUp << std::endl;
        d["typePowerUp"] = cPowerUpCar->typePowerUp;

        //RenderFacadeManager::GetInstance()->GetRenderFacade()->FacadeUpdatePowerUpHUD(d);
        EventManager::GetInstance()->AddEventMulti(Event{EventType::UPDATE_POWERUP_HUD, d});

    }
    //cPowerUp->typePowerUp = dynamic_cast<typeCPowerUp*>(indx);
}

void ManCar::CatchPowerUpAI(DataMap d) {
    srand(time(NULL));
    int indx = rand() % 100+1;
    if(indx <= 5)                       // 5%
        indx = 1;
    else if(indx > 5 && indx <= 20)     // 15%
        indx = 2;
    else if(indx > 20 && indx <= 45)    // 25%
        indx = 3;
    else if(indx > 45 && indx <= 60)    // 15%
        indx = 4;
    else if(indx > 60 && indx <= 75)    // 15%
        indx = 5;
    else if(indx > 75)                  //  25%
        indx = 6;
    //indx = 5;
    //indx=3;
    auto cPowerUpCar = static_cast<CPowerUp*>(any_cast<Entity*>(d["actualCar"])->GetComponent(CompType::PowerUpComp).get());
    if(cPowerUpCar->typePowerUp == typeCPowerUp::None){
        cPowerUpCar->typePowerUp = (typeCPowerUp)indx;
        std::cout << "Mi super powerUp es:   " << (int)cPowerUpCar->typePowerUp << std::endl;
    }
    //cPowerUp->typePowerUp = dynamic_cast<typeCPowerUp*>(indx);
}




bool ManCar::carInVisionRange(Entity* actualCar, Entity* otherCar){
    float seeCar = false;
    // calcular un desplazamiento para ser en tercera persona
    auto cTransformableActual = static_cast<CTransformable*>(actualCar->GetComponent(CompType::TransformableComp).get());
    float posXActualCar = cTransformableActual->position.x + 40 * cos((cTransformableActual->rotation.y * PI) / 180.0);
    float posZActualCar = cTransformableActual->position.z - 40 * sin((cTransformableActual->rotation.y * PI) / 180.0);

    // vector between actualCar and otherCar
    
    auto cTransformableOther = static_cast<CTransformable*>(otherCar->GetComponent(CompType::TransformableComp).get());
    float vetorWaypointX = (cTransformableOther->position.x - posXActualCar);
    float vetorWaypointZ = (cTransformableOther->position.z - posZActualCar);

    // calculate position rotated of otherCar atan2
    float valueAtan2 = atan2(vetorWaypointZ,vetorWaypointX)*180/PI;
    valueAtan2 = 180.0 - valueAtan2; // se le restan ya que el eje empieza en el lado contrario 
    if(valueAtan2<0)
        valueAtan2 += 360;

    //compare with actualCar actualRotation
    if(cTransformableActual->rotation.y-rangeVision >= 0 && cTransformableActual->rotation.y+rangeVision<360){
        if(cTransformableActual->rotation.y-rangeVision<valueAtan2 && cTransformableActual->rotation.y+rangeVision>valueAtan2){
            seeCar=true;
        }
    }else{  // coge el angulo 0 de por medio
        float rotMin = cTransformableActual->rotation.y-rangeVision;
        float rotMax = cTransformableActual->rotation.y+rangeVision;
        if(cTransformableActual->rotation.y-rangeVision < 0)
            rotMin += 360;
        if(cTransformableActual->rotation.y+rangeVision >= 360)
            rotMax -= 360;
        if(rotMin<valueAtan2 || rotMax>valueAtan2){
            seeCar=true;
        }
    }
    return seeCar;
}


// comprobamos si tenemos algun coche en el rango de vision
bool ManCar::anyCarInVisionRange(Entity* actualCar){
    bool seeCar = false;
    for(shared_ptr<Entity> carAI : CarAIs){
        if(actualCar!=carAI.get()){
            if(carInVisionRange(actualCar,carAI.get()) == true){
                seeCar = true;
            }
        }
    }
    // comprobamos el player
    if(car.get()!=actualCar){
        if(carInVisionRange(actualCar,car.get()) == true){
            seeCar = true;
        }
    }
    return seeCar;
}





void ManCar::TurnLeftCar(DataMap d) {
    // cout << "Han llamado izquierda" << endl;
    physics->TurnLeft(car.get(), cam);
}

void ManCar::NotTurning(DataMap d) {
    // cout << "Han llamado not turning" << endl;
    physics->NotTurning(car.get(), cam);
}

void ManCar::Decelerate(DataMap d) {
    // cout << "Han llamado decel" << endl;
    physics->Decelerate(car.get(), cam);
}

void ManCar::NotAcceleratingOrDecelerating(DataMap d) {
    // cout << "Han llamado notaccel" << endl;
    physics->NotAcceleratingOrDecelerating(car.get(), cam);
}

void ManCar::TurnRightCar(DataMap d) {
    // cout << "Han llamado derecha" << endl;
    physics->TurnRight(car.get(), cam);
}

void ManCar::AccelerateCar(DataMap d) {
    // cout << "Estamos en AccelerateCar" << endl;
    // int mint = any_cast<int>(d["int"]);
    // cout << "Hemos obtenido el int" << mint << endl;
    // float mfloat = any_cast<float>(d["float"]);
    // cout << "Hemos obtenido el float" << endl;
    // vector<int> mvect = any_cast<vector<int>>(d["vector"]);
    // cout << "Hemos recibido por evento un int=" << mint << " un float=" << mfloat << " y un vector de int con size=" << mvect.size() << endl;
    
    physics->Accelerate(car.get(), cam);
}

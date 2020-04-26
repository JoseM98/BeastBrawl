#include "SystemBtDecisionMove.h"


#include <behaviourTree/behaviourTree.h>
#include <behaviourTree/selector.h>
#include <behaviourTree/sequence.h>
#include <behaviourTree/decorator.h>

#include <behaviourTree/Blackboard.h>
#include <Components/CTotem.h>
#include <Components/CCurrentNavMesh.h>
#include <Components/CBrainAI.h>

void SystemBtDecisionMove::AddManager(Manager &m) {
    managers.push_back(&m);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
//                           COMPROBAR BEHAVIOR TREE
////////////////////////////////////////////////////////////////////////////////////////////////////
///// DECORATORS //////
struct Facil2times_dm : public Decorator {
    uint32_t totalTried = 2;
    uint32_t numTried = 0;
    virtual bool run(Blackboard* blackboard) override {
        if (numTried >= totalTried)
            return false;
        numTried++;
        return getChild()->run(blackboard);
    }
};

// ----------------------------------------   CONDICIONES -------------------------------------------
//CONDICION Tenemos powerUp?
struct HavePowerUp_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //return true;
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if(cPowerUp->typePowerUp == typeCPowerUp::None){
            //std::cout << "No tenemos powerUp" << std::endl;
            return false;
        }
        return true;
    }
};
//CONDICION esa el Totem suelto por el mapa?
struct TotemAlone_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cTotem = static_cast<CTotem*>(blackboard->manTotems->GetEntities()[0]->GetComponent(CompType::TotemComp).get());
        return cTotem->active;
    }
};
//CONDICION Tenemos un powerUp de ataque? Melon o Banana
struct HavePowerUpAttack_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cPowerUp = static_cast<CPowerUp*>(blackboard->actualCar->GetComponent(CompType::PowerUpComp).get());
        if( cPowerUp->typePowerUp == typeCPowerUp::MelonMolon || 
            cPowerUp->typePowerUp == typeCPowerUp::TeleBanana ||
            cPowerUp->typePowerUp == typeCPowerUp::SuperMegaNitro  ){
            //std::cout << "El powerUp era o Melon o Banana" << std::endl;
            return true;
        }
        return false;
    }
};

struct HaveTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        auto cTotem = static_cast<CTotem*>(actualCar->GetComponent(CompType::TotemComp).get());
        if(cTotem->active == true){
            // nosotros tenemos el totem
            return true;
        }
        return false;
    }
};

// TODO: NO IMPLEMENTADO 
struct SeeTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //auto actualCar = blackboard->actualCar;
        //auto cTotem = static_cast<CTotem*>(actualCar->GetComponent(CompType::TotemComp).get());
        return false;
    }
};

// TODO: NO IMPLEMENTADO
struct SeePowerUp_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //auto actualCar = blackboard->actualCar;
        return false;
    }
};

// TODO: NO IMPLEMENTADO
struct Think_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        // lo que tendremos que hacer es acceder a un compoentne es ponerlo a false para wue no haga el LoD 
        // y por tanto no actualice su movimento durante estas iteraciones
        //auto actualCar = blackboard->actualCar;
        return true;
    }
};

// Condicion para saber si estamos en el mismo navmesh que el totem
struct SameNavMeshCT_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cTotem = static_cast<CTotem*>(blackboard->manTotems->GetEntities()[0]->GetComponent(CompType::TotemComp).get());
        if(cTotem->active){
            auto cCurrendNavMeshCar = static_cast<CCurrentNavMesh*>(blackboard->actualCar->GetComponent(CompType::CurrentNavMeshComp).get());
            auto cCurrentNavMeshTotem = static_cast<CCurrentNavMesh*>(blackboard->manTotems->GetEntities()[0].get()->GetComponent(CompType::CurrentNavMeshComp).get());
            if(cCurrendNavMeshCar->currentNavMesh == cCurrentNavMeshTotem->currentNavMesh){
                // estamos en el mismo navmesh que el totem
                return true;
            }else{
                // no estamos en el mismo navmesh
                return false;
            }
        }
        // nisiquiera esta en el suelo
        return false;
    }
};



// ----------------------------------------     ACCIONES   -------------------------------------------
//ACCION movernos al totem
struct MoveToTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cTotem = static_cast<CTotem*>(blackboard->manTotems->GetEntities()[0]->GetComponent(CompType::TotemComp).get());
        if(cTotem->active){
            auto cTransformable = static_cast<CTransformable*>(blackboard->manTotems->GetEntities()[0].get()->GetComponent(CompType::TransformableComp).get());
            //DataMap* dataTotem = new DataMap();
            shared_ptr<DataMap> dataTotem = make_shared<DataMap>();                                                                
            (*dataTotem)[ACTUAL_CAR] = blackboard->actualCar;              
            (*dataTotem)[POS_DESTINATION] = cTransformable->position;                                        
            EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataTotem}); 
            return true;
        }
        return false;
    }
};

struct MoveToNavMeshTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto cTotem = static_cast<CTotem*>(blackboard->manTotems->GetEntities()[0]->GetComponent(CompType::TotemComp).get());
        if(cTotem->active){
            //auto cCurrendNavMeshCar = static_cast<CCurrentNavMesh*>(blackboard->actualCar->GetComponent(CompType::CurrentNavMeshComp).get());
            auto cCurrentNavMeshTotem = static_cast<CCurrentNavMesh*>(blackboard->manTotems->GetEntities()[0].get()->GetComponent(CompType::CurrentNavMeshComp).get());

            auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
            if(cBrainAI->targetNavMesh != cCurrentNavMeshTotem->currentNavMesh){
                cBrainAI->targetNavMesh = cCurrentNavMeshTotem->currentNavMesh;
                shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();      
                (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
                (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;
                (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;  
                (*dataPowerUp)[ID_DESTINATION] = cCurrentNavMeshTotem->currentNavMesh;                                                                                                   
                EventManager::GetInstance().AddEventMulti(Event{EventType::CALCULATE_PATH_TO_NAVMESH, dataPowerUp}); 
            }  
            return true;
        }
        return false;
    }
};





//ACCION huir ya que tenemos el totem (seguir en el random que teniamos antes de ir de power up en power up, de momento)
// TODO: como no esta implementado actualmente nos movemos entre los powerUps para no quedarnos parados
struct EscapeWithTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //std::cout << "Vamos a movernos por los powerUps UUUEEEEPAA" << std::endl;    
        shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();
        (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
        (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;  
        (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;                                                                                                      
        EventManager::GetInstance().AddEventMulti(Event{EventType::MOVE_TO_POWERUP, dataPowerUp}); 
        return true;
    }
};
//ACCION movernos a un powerUp (seguir en el random que teniamos antes de ir de power up en power up)
// TODO: ... para hacerlo mas autonomo --> nos moveremos aun powerUp aleatorio del mapa.. mas adelante sera al mas cercano/ al que este mirando
struct MoveToPowerUp_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        //std::cout << "Vamos a movernos por los powerUps UUUEEEEPAA" << std::endl;
        shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();      
        (*dataPowerUp)[ACTUAL_CAR] = blackboard->actualCar;     
        (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;  
        (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;                                                                                                      
        EventManager::GetInstance().AddEventMulti(Event{EventType::MOVE_TO_POWERUP, dataPowerUp}); 
        return true;
    }
};
//ACCION movernos al jugador con Totem 
struct MoveToCarTotem_dm : public behaviourTree {
    virtual bool run(Blackboard* blackboard) override {
        auto actualCar = blackboard->actualCar;
        for(const auto& actualAI : blackboard->manCars->GetEntities()){
            if (static_cast<Car*>(actualAI.get())->GetTypeCar() == TypeCar::CarAI){
                auto cTotem = static_cast<CTotem*>(actualAI->GetComponent(CompType::TotemComp).get());
                if(cTotem->active == true){
                    auto cCurrendNavMeshCar = static_cast<CCurrentNavMesh*>(actualCar->GetComponent(CompType::CurrentNavMeshComp).get());
                    auto cCurrendNavMeshCarAI = static_cast<CCurrentNavMesh*>(actualAI->GetComponent(CompType::CurrentNavMeshComp).get());
                    if(cCurrendNavMeshCar->currentNavMesh == cCurrendNavMeshCarAI->currentNavMesh){
                        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
                        cBrainAI->targetNavMesh = -1;
                        //if(actualCar != actualAI.get()){
                        auto cTransformable = static_cast<CTransformable*>(actualAI->GetComponent(CompType::TransformableComp).get());
                        shared_ptr<DataMap> dataCarTotem = make_shared<DataMap>();                                                                    
                        (*dataCarTotem)[ACTUAL_CAR] = actualCar;             
                        (*dataCarTotem)[POS_DESTINATION] = cTransformable->position;                                        
                        EventManager::GetInstance().AddEventMulti(Event{EventType::CHANGE_DESTINATION, dataCarTotem});

                        return true;
                    }else{
                        auto cBrainAI = static_cast<CBrainAI*>(blackboard->actualCar->GetComponent(CompType::BrainAIComp).get());
                        auto cCurrendNavMeshCarAI = static_cast<CCurrentNavMesh*>(actualAI->GetComponent(CompType::CurrentNavMeshComp).get());
                        if(cBrainAI->targetNavMesh != cCurrendNavMeshCarAI->currentNavMesh){
                            cBrainAI->targetNavMesh = cCurrendNavMeshCarAI->currentNavMesh;

                            shared_ptr<DataMap> dataPowerUp = make_shared<DataMap>();    
                            (*dataPowerUp)[ACTUAL_CAR] = actualCar;     
                            (*dataPowerUp)[MAN_WAYPOINTS] = blackboard->manWayPoint;
                            (*dataPowerUp)[MAN_NAVMESH] = blackboard->manNavMesh;   
                            (*dataPowerUp)[ID_DESTINATION] = cCurrendNavMeshCarAI->currentNavMesh;                                                                
                            EventManager::GetInstance().AddEventMulti(Event{EventType::CALCULATE_PATH_TO_NAVMESH, dataPowerUp}); 
                        }

                        return true;
                    }
                }
            }
        }
        return false;     
    }
};
// NOTA: recordar que tenemos el otro Behaviour tree de tirar powerUp y por tanto lo seguiremos
// hasta que lo veamos, una vez visto le lanzaemos el powerUp e iremos a por el Totem u otro PowerUp



//    None,               // 0
//    RoboJorobo,         // 1
//    SuperMegaNitro,     // 2
//    PudinDeFrambuesa,   // 3
//    EscudoMerluzo,      // 4
//    TeleBanana,         // 5
//    MelonMolon          // 6
SystemBtDecisionMove::SystemBtDecisionMove(){

    //   CREACION DEL ARBOL DE DECISIONES

    selectorBehaviourTree = make_unique<selector>();

    // selector TOTEM EN EL SUELO
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    unique_ptr<selector> selector1  = make_unique<selector>();
        unique_ptr<selector> selector11 = make_unique<selector>();
            unique_ptr<sequence> sequence11_1   = make_unique<sequence>();
                unique_ptr<SeeTotem_dm> c_SeeTotem = make_unique<SeeTotem_dm>();
                unique_ptr<MoveToTotem_dm> a_MoveToTotem = make_unique<MoveToTotem_dm>();
            unique_ptr<sequence> sequence11_2   = make_unique<sequence>();
                unique_ptr<SameNavMeshCT_dm> c_SameNavMeshCT = make_unique<SameNavMeshCT_dm>();
                //unique_ptr<MoveToTotem_dm> a_MoveToTotem = make_unique<MoveToTotem_dm>(); --> no vamos a crear dos acciones iguales
            unique_ptr<selector> selector111    = make_unique<selector>();
                unique_ptr<sequence> sequence111_1 = make_unique<sequence>(); 
                    unique_ptr<SeePowerUp_dm> c_SeePowerUp = make_unique<SeePowerUp_dm>();
                    //unique_ptr<MoveToTotem_dm> a_MoveToTotem = make_unique<MoveToTotem_dm>(); --> es la misma, mo la voy a crear otra vez
                unique_ptr<selector> selector1111 = make_unique<selector>(); 
                    // algo con lo que pensemos las dos priemra veces
                    unique_ptr<Facil2times_dm> d_Facil2TimesMT = make_unique<Facil2times_dm>();
                        unique_ptr<MoveToNavMeshTotem_dm> a_MoveToNavMeshTotem = make_unique<MoveToNavMeshTotem_dm>();
                    unique_ptr<Think_dm> a_Thinck = make_unique<Think_dm>(); 
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 

    //shared_ptr<sequence> sequence1 = make_shared<sequence>();
    //    shared_ptr<TotemAlone_dm> c_totemAlone =   make_shared<TotemAlone_dm>();
    //    shared_ptr<MoveToTotem_dm> a_moveToTotem = make_shared<MoveToTotem_dm>();
    //shared_ptr<sequence> sequence2 = make_shared<sequence>();
    //    shared_ptr<HaveTotem_dm> c_haveTotem =   make_shared<HaveTotem_dm>();
    //    shared_ptr<EscapeWithTotem_dm> a_escapeWithTotem = make_shared<EscapeWithTotem_dm>();
    //shared_ptr<sequence> sequence3 =    make_shared<sequence>();
    //    shared_ptr<Inverter_dm> m_inverter1 =  make_shared<Inverter_dm>();
    //        shared_ptr<HavePowerUp_dm> c_havePowerUp = make_shared<HavePowerUp_dm>(); 
    //    shared_ptr<MoveToPowerUp_dm> a_moveToPowerUp = make_shared<MoveToPowerUp_dm>(); 
    //shared_ptr<sequence> sequence4  = make_shared<sequence>();
    //    shared_ptr<HavePowerUpAttack_dm> c_havePowerUpAttack = make_shared<HavePowerUpAttack_dm>();
    //    shared_ptr<MoveToCarTotem_dm> a_moveToCarTotem =       make_shared<MoveToCarTotem_dm>();
//
//
    //std::cout << "------------------------ Behaviour Move To --------------------------------" << std::endl;
    //// ASIGNACION DEL ARBOL DE DECISIONES
//
    //// el totem esta en el suelo?
    //selectorBehaviourTree->addChild(sequence1);
    //// tengo yo el totem?
    //selectorBehaviourTree->addChild(sequence2);
    //// vamos a coger PowerUps si no tenemos
    //selectorBehaviourTree->addChild(sequence3);
    //// si el powerUp es de ataque voy a por el que tiene el totem
    //selectorBehaviourTree->addChild(sequence4);
    //// TODO somos una IA que tiene el powerUp y obvimanete no nos vamos a perseguir a nosotros mismos
    ////selectorBehaviourTree->addChild(a_moveToPowerUp);
//
    //sequence1->addChild(c_totemAlone);
    //sequence1->addChild(a_moveToTotem);
//
    //sequence2->addChild(c_haveTotem);
    //sequence2->addChild(a_escapeWithTotem);
//
    //sequence3->addChild(m_inverter1);
    //    m_inverter1->addChild(c_havePowerUp);
    //sequence3->addChild(a_moveToPowerUp);
//
    //sequence4->addChild(c_havePowerUpAttack);
    //sequence4->addChild(a_moveToCarTotem);
    
}




void SystemBtDecisionMove::update(CarAI* actualCar){
    unique_ptr<Blackboard> blackboard = make_unique<Blackboard>(actualCar, static_cast<ManCar*>(managers[0]), static_cast<ManPowerUp*>(managers[1]), 
        static_cast<ManBoxPowerUp*>(managers[2]), static_cast<ManTotem*>(managers[3]), static_cast<ManWayPoint*>(managers[4]), static_cast<ManNavMesh*>(managers[5]) );
    
    

    selectorBehaviourTree->run(blackboard.get());
}
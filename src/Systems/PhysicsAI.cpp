#include "PhysicsAI.h"

#include "../Entities/Car.h"
#include "../Entities/CarAI.h"
#include "../Entities/WayPoint.h"


#include "../Components/CPowerUp.h"
#include "../Components/CShield.h"
#include "../Components/CTotem.h"
#include "../Components/CRoboJorobo.h"
#include "../Components/CNitro.h"
#include "../Components/CPath.h"
#include "../Components/CWayPointEdges.h"

#include "../Facade/Render/RenderFacadeManager.h"
#include "../Game.h"
#include "../Managers/Manager.h"
#include "../Managers/ManPowerUp.h"
#include "../Managers/ManBoxPowerUp.h"
#include "../Managers/ManTotem.h"
#include "../Managers/ManWayPoint.h"



PhysicsAI::PhysicsAI(){

}



void PhysicsAI::Update(CarAI* carAI, ManWayPoint* graph){
    UpdateCarPowerUps(carAI);

    UpdateDikkstra(carAI, graph);    
}


void PhysicsAI::UpdateCarPowerUps(CarAI* carAI){
    auto cTotem = static_cast<CTotem*>(carAI->GetComponent(CompType::TotemComp).get());
    if(cTotem->active){
        cTotem->accumulatedTime +=  duration_cast<milliseconds>(system_clock::now() - cTotem->timeStart).count();
        cTotem->timeStart = system_clock::now();
    }

    if(cTotem->accumulatedTime/1000.0 > cTotem->durationTime/1000.0){
        cout << "Has ganado\n";
        Game::GetInstance()->SetState(State::ENDRACE);
    }

    // Actualiza el componente nitro
    auto cNitro = static_cast<CNitro *>(carAI->GetComponent(CompType::NitroComp).get());
    if(cNitro->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cNitro->timeStart).count() > cNitro->durationTime){  // comprueba el tiempo desde que se lanzo
        cNitro->deactivePowerUp();
    }

    // Actualiza el componente escudo
    auto cShield = static_cast<CShield *>(carAI->GetComponent(CompType::ShieldComp).get());
    if(cShield->activePowerUp==true && duration_cast<milliseconds>(system_clock::now() - cShield->timeStart).count() > cShield->durationTime){  // comprueba el tiempo desde que se lanzo
        cShield->deactivePowerUp();
    }
}



void PhysicsAI::UpdateDikkstra(CarAI* carAI, ManWayPoint* graph){
    //Guardamos en varAIbles los componentes
	auto cTransformable = static_cast<CTransformable*>(carAI->GetComponent(CompType::TransformableComp).get());
    //auto cWayPoint     = static_cast<CWayPoint*>(carAI->GetComponent(CompType::WayPointComp).get());
    //float radious = cWayPoint->radious;
    auto cPosDestination     = static_cast<CPosDestination*>(carAI->GetComponent(CompType::PosDestination).get());
    float radious = cPosDestination->radious;

    //Vamos a comprobar si esta en el rango del waypoint
    if((cPosDestination->position.z - radious) < cTransformable->position.z && (cPosDestination->position.z + radious) >= cTransformable->position.z 
        && (cPosDestination->position.x - radious) < cTransformable->position.x && (cPosDestination->position.x + radious) >= cTransformable->position.x){
            
        //Tenemos que comprobar si le quedan mas nodos que visitar en el path
        auto cPath = static_cast<CPath*>(carAI->GetComponent(CompType::PathComp).get());
        if(!cPath->stackPath.empty()){
            auto actualNode = cPath->stackPath.top();
            cPath->stackPath.pop();
            //cout << "Llegamos al WayPoint: " << actualNode << endl;
            if(!cPath->stackPath.empty()){
                //Le asignamos el WayPoint siguiente del path (graph->GetEntities()[cPath->stackPath.top()])
                auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[cPath->stackPath.top()]->GetComponent(CompType::WayPointComp).get());
                cPosDestination->position = cWayPoint->position;

                carAI->SetDestination(cPosDestination);
            }else{
                //Si esta vacio es que ha acabado el path y recalculamos otro
                //TO-DO: de momento le recalculamos otro aleatorio
                // habra que llamar al arbol de decisiones broooo
                int indx;
                do{
                    indx = rand() % graph->GetEntities().size();

                }while(indx==actualNode);

                //COMPROBAMOS DIJKSTRA
                auto path = Dijkstra(graph,actualNode,indx);
                carAI->SetPath(path);

                auto cWayPoint = static_cast<CWayPoint*>(graph->GetEntities()[path.top()]->GetComponent(CompType::WayPointComp).get());
                carAI->SetWayPoint(cWayPoint);
            }          
        }
    }
}



std::stack<int> PhysicsAI::Dijkstra(ManWayPoint* _graph, int start, int end) {
    //cout << "----------------------------------\n";
    //Convertir ManWayPoint en una matriz de adyacencia
    int size = _graph->GetEntities().size();
    float graph[size][size];

    //Rellenamos de 0 el grafo
    for(int i = 0; i < size; ++i){
        for(int j = 0; j < size; ++j){
            graph[i][j] = INT_MAX;
        }
    }

    //Ponemos los costes pertinentes en la matriz de adyacencia
    //TODO: Cambiar esto para tenerlo guardado en una entidad o algo y no hacerlo cada calculo de Dijkstra
    for(auto node : _graph->GetEntities()){
        auto cWayPoint = static_cast<CWayPoint*>(node->GetComponent(CompType::WayPointComp).get());
        auto cWayPointEdges = static_cast<CWayPointEdges*>(node->GetComponent(CompType::WayPointEdgesComp).get());

        for(auto edge : cWayPointEdges->edges){
            graph[cWayPoint->id][edge.to] = edge.cost;
        }
    }

    //Comenzamos Dijkstra
    float distanceFromStart[size],pred[size];
    int visited[size],count,minDistanceFromStart,nextClosestNode,i;

    for(i=0;i<size;i++) {
        distanceFromStart[i] = graph[start][i];  //Metemos las ponderaciones a los nodos desde el que iniciamos(Si no tiene es = INT_MAX)
        pred[i] = start;                
        visited[i] = 0;
    }

    //La distancia a si mismo es siempre 0
    distanceFromStart[start]=0; 
    visited[start]=1;
    count=1;

    while(count<size-1) {
        minDistanceFromStart=INT_MAX;
        for(i=0;i<size;i++){
            if(distanceFromStart[i] < minDistanceFromStart && !visited[i]) {
                //Si la distancia al nodo i es menor que la minDistanceFromStart y no esta visitado
                //Recordatorio: Si nuestro nodo start no esta conectado con i entonces distanceFromStart[1] = INT_MAX y no entrará aquí
                minDistanceFromStart=distanceFromStart[i]; // Distancia al nodo adyacente mas cercano
                nextClosestNode=i; //Siguiente nodo adyacente mas cercano
            }
        }

        visited[nextClosestNode]=1;

        for(i=0;i<size;i++){
            if(!visited[i]){
                //Si la distancia entre (start y nodo i) es mayor que (start y su nodo adyacente) + (su nodo adyacente hasta i)
                //P.E: ¿De 1 -> 3 es mayor que de 1 -> 2 -> 3?
                if(minDistanceFromStart + graph[nextClosestNode][i] < distanceFromStart[i]) {
                    distanceFromStart[i]=minDistanceFromStart + graph[nextClosestNode][i];
                    pred[i]=nextClosestNode; //Nos guardamos en pred[i] el nodo por el que mas rapido se llega a él (nextClosestNode)
                }
            }
        }
        count++;
    }

    stack<int> path;
    int aux = end;
    path.push(aux); //Para guardarnos el final del path
    //Recorremos pred recursivamente hasta que pred[aux] sea el nodo start
    while(aux!=start){
        if(pred[aux]==start) break; //Para que no nos añada el nodo start
        path.push(pred[aux]);
        aux = pred[aux];
    }

    //cout << "Nuevo Path: ";
    stack<int> pathAux(path);
    while(!pathAux.empty()){
        auto node = pathAux.top();
        pathAux.pop();

        //cout << node << " - ";
    }

    //cout << "\n---------------\n";

    return path;

    //cout << "\n\n\n";
}
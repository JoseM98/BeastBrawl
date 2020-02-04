#include "EventManager.h"
#include <iostream>

// const shared_ptr<EventManager> EventManager::instance = make_shared<EventManager>();
// shared_ptr<EventManager> EventManager::IGetInstance() {
//     //static EventManager instance;
//     // if(instance==nullptr){
//     //     instance = make_shared<EventManager>();
//     // }
//     return instance;
// }

EventManager& EventManager::GetInstance() {
    static EventManager instance;
    return instance;
}   

//Realiza y vacia todos los eventos que estaban añadidos
//O(n)
//Acceso: O(1)
void EventManager::Update() {

    while (!eventList.empty()) {
        Event e = eventList.front();  //Cojemos el primero en la lista
        eventList.pop_front();        // Lo sacamos de la lista

        //std::cout << "Procesando evento con prioridad: " << e.type << std::endl;

        //Tratamos el evento
        auto mapByType = eventListenerMap.find(e.type);

        if (mapByType != eventListenerMap.end()) {
            auto eventVector = mapByType->second;  // El vector de listeners del mapa segun el EventType

            for (const Listener& listener : eventVector) {
                listener.callback(e.data.get());
            }
        }
    }
}

//Añade un evento a la cola de eventos
// O(n) -> Recorremos la lista iterativamente hasta encontrar el valor directamente superior
void EventManager::AddEventMulti(const Event& e) {
    //FIXME: Descomentar esto para que funcione con cola
    //eventQueue.push(e);

    std::list<Event>::iterator it;

    //Si es el primer evento lo añadimos al comienzo
    if (eventList.empty()) {
        eventList.push_front(e);
    } else {
        //TODO: Implementar un algoritmo de busqueda e insercion mas optimo
        //Movemos el iterator hasta posicionarlo antes del siguiente valor mas grande
        for (it = eventList.begin(); it != eventList.end(); ++it) {
            if (e.type <= it->type) {
                break;
            }
        }

        eventList.insert(it, e);
    }
}

// Añade un listener al mapa
void EventManager::SuscribeMulti(const Listener& listener) {
    //Vamos a ver si tiene ya alguno de este tipo
    auto iterator = eventListenerMap.find(listener.type);

    //Si entra es que no habia uno ya creado de ese tipo
    if (iterator == eventListenerMap.end()) {
        ListenerVector listenerVector;
        iterator = eventListenerMap.insert(std::pair<EventType, ListenerVector>(listener.type, listenerVector)).first;
    }

    iterator->second.push_back(listener);
    std::cout << "Añadido el Listener: " << listener.name << "\n";
}

// Añade un listener al mapa
void EventManager::Suscribe(const Listener& listener) {
    //Vamos a ver si tiene ya alguno de este tipo
    auto iterator = eventListenerMap.find(listener.type);

    //Si entra es que no habia uno ya creado de ese tipo
    if (iterator == eventListenerMap.end()) {
        ListenerVector listenerVector;
        iterator = eventListenerMap.insert(std::pair<EventType, ListenerVector>(listener.type, listenerVector)).first;
    }

    iterator->second.push_back(listener);
    cout << "Añadido el Listener: " << listener.name << "\n";
}

//TODO: Esto hay que mejorarlo muchisimo pero no controlo aun demasiado tema de punteros e iterators
void EventManager::UnSuscribeMulti(EventType eType, const string& listenerName) {
    ShowSuscribers();
    auto mapByType = eventListenerMap.find(eType);

    if (mapByType != eventListenerMap.end()) {
        auto listenerVector = mapByType->second;  // El vector de listeners del mapa segun el EventType

        for (uint32_t i = 0; i < listenerVector.size(); ++i) {
            if (listenerVector.at(i).name.compare(listenerName) == 0) {
                cout << "Borramos el Listener de tipo: " << listenerVector.at(i).type << " con nombre " << listenerVector.at(i).name << endl;
                listenerVector.erase(listenerVector.begin() + i);
                break;
            }
        }

        //TODO: Esto es mucho gasto el borrar todo el vector de listeners y volver a introducirlo
        eventListenerMap.erase(mapByType);
        eventListenerMap.insert(pair<EventType, ListenerVector>(eType, listenerVector)).first;
    }
    ShowSuscribers();
}

void EventManager::ClearEvents() {
    eventList.clear();
}

void EventManager::ClearListeners() {
    eventListenerMap.clear();
}

//DEBUG: Recorre y muestra todos los suscriptores
//Fuente: https://stackoverflow.com/questions/26281979/c-loop-through-map
void EventManager::ShowSuscribers() {
    for (auto const& mapByType : eventListenerMap) {
        std::cout << "Tipo de evento: " << mapByType.first << "\n";

        for (auto const& listener : mapByType.second) {
            cout << listener.name << " - ";
        }
        cout << "\n";
    }
}

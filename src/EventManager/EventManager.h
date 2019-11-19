#pragma once

#include "Event.h"
#include <queue>
#include <map>
#include <list>

class EventManager{

    public:

        EventManager() {}
        ~EventManager() {}
        static EventManager& GetInstance();
        void Suscribe(Listener);
        void UnSuscribe(EventType, string);
        void AddEvent(Event);
        void Update();

    private:
        void ShowSuscribers();
        //FIXME: Al final tendremos que escoger si usar la cola o la lista
        queue<Event> eventQueue;
        list<Event> eventList;
        
        map<EventType, ListenerVector> eventListenerMap; //Mapa con el tipo de evento y un vector con los listeners suscritos a dicho evento

};
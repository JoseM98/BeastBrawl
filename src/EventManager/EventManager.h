#pragma once
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include "Event.h"

using namespace std;

class EventManager {
   public:
    EventManager(EventManager const&) = delete;
    void operator=(EventManager const&) = delete;
    ~EventManager(){};

    static EventManager& GetInstance();
    void SubscribeMulti(Listener);
    void Subscribe(Listener);
    void UnSubscribeMulti(EventType, string);
    void AddEventMulti(Event);
    void Update();
    void ClearEvents();
    void ClearListeners();
    // void AddStrand(boost::asio::io_context::strand &strand);

   private:
    EventManager() : id_single(0) {  };

    std::thread::id idHiloCero{0};
    std::atomic<std::thread::id> idOcupado {idHiloCero};

    uint32_t id_single;
    // static const EventManager instance;
    void ShowSuscribers();
    //FIXME: Al final tendremos que escoger si usar la cola o la lista
    //std::queue<Event> eventQueue;
    list<Event> events;  // lista de eventos posibles
    // tbb::concurrent_vector<Event> eventList;  // lista de eventos posibles
    // tbb::concurrent_queue<Event> eventList;  // lista de eventos posibles

    map<EventType, ListenerVector> listeners;  //Mapa con el tipo de evento y un vector con los listeners suscritos a dicho evento
    // tbb::concurrent_map<EventType, ListenerVector> eventListenerMap;  //Mapa con el tipo de evento y un vector con los listeners suscritos a dicho evento
    std::mutex mutex_vector;
};
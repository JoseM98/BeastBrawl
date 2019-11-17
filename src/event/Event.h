#include <cstdint>
#include <vector>


//El orden de los enums define la prioridad del evento
enum EventType{
    PRIORIDAD1,
    PRIORIDAD2,
    PRIORIDAD3

};

struct Data{
    uint16_t id;
};

struct Event {

    EventType type;
    Data data;

};

struct Listener{
    EventType type;
    void (*function)(Data); // Puntero a funcion 
};

typedef std::vector<Listener> ListenerVector; // Se usará para poder tener varios listener del mismo EventType en el hash map
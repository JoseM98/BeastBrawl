#pragma once

struct Blackboard;
enum class stateNode { Success, Failure, InProcess };

class behaviourTree{

    public:
        // run() hace referencia a la accion a realizar 
		virtual bool run(Blackboard*) = 0;

    private:
        stateNode   state;  

};

// PADRE --> run() no lo implementa
// COMPOSITE ---> selector o secuenca
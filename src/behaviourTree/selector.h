#pragma once

#include "composite.h"

struct Blackboard;

using namespace std;

class selector: public composite {

  public:
        //Aun que pone que es redundante, Jose dice que no se quite el 'virtual'. ¿Por qué? No se, preguntale a él.
	    virtual bool run(Blackboard* blackboard) override;

};
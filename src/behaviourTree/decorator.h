#pragma once

#include "behaviourTree.h"
#include <memory>

struct Blackboard;

using namespace std;

class Decorator: public behaviourTree{
    public:
        virtual bool run(Blackboard*) = 0;
        void addChild(shared_ptr<behaviourTree>);
        shared_ptr<behaviourTree> getChild(){ return child; };

    private:
        shared_ptr<behaviourTree> child;
};


#include "decorator.h"


void Decorator::addChild(shared_ptr<behaviourTree> c){
	child= std::move(c);
}





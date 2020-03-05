#pragma once

#include "composite.h"

struct Blackboard;

using namespace std;


class sequence: public composite{
  
  public:
	  virtual bool run(Blackboard* blackboard) override;

};
#include "selector.h"
#include "Blackboard.h"

#include <memory>

bool selector::run(Blackboard* blackboard){
	for (const shared_ptr<behaviourTree>& child : getChildren()) {          // The generic Selector implementation
		if (child->run(blackboard))  			                            // If one child succeeds, the entire operation run() succeeds.  Failure only results if all children fail.
				return true;
	}
	return false;  									                        // All children failed so the entire run() operation fails.
}
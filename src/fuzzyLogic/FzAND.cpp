#include "FzAND.h"

#include <vector>


//FzAND::FzAND(FuzzyTerm& one, FuzzyTerm& two){

//}
// crear un AND de 3
//FzAND::FzAND(FuzzyTerm& one, FuzzyTerm& two, FuzzyTerm& three){
//}
// crear un AND de 4
//FzAND::FzAND(FuzzyTerm& one, FuzzyTerm& two, FuzzyTerm& three, FuzzyTerm& four){
//}

/*
EJEMPLO CLONE()
    virtual Implementation* clone() const override
    {
        return new Implementation(*this);
    }
*/
// DUDA como hago el clon?
//FuzzyTerm* FzAND::Clone()const{
//	return new FzAND(*this);
//}
//retrieves the degree of membership of the term
// puerta AND = minimo
double FzAND::GetDOM()const{

	auto curTerm = arrayFzAND.begin();
	// DUDA en el ejemplo se iguala a 0, supomgo que esta mal
	double minDOM = (*curTerm)->GetDOM();
	for (curTerm = arrayFzAND.begin(); curTerm != arrayFzAND.end(); ++curTerm){
		if( (*curTerm)->GetDOM() < minDOM  )
			minDOM = (*curTerm)->GetDOM();
	}
	return minDOM;
}
//clears the degree of membership of the term
void FzAND::ClearDOM(){
	auto curTerm = arrayFzAND.begin();
	for (curTerm = arrayFzAND.begin(); curTerm != arrayFzAND.end(); ++curTerm){
		(*curTerm)->ClearDOM();
	}	
}
// method for updating the DOM of a consequent when a rule fires
void FzAND::ORwithDOM(double val){
	auto curTerm = arrayFzAND.begin();
	for (curTerm = arrayFzAND.begin(); curTerm != arrayFzAND.end(); ++curTerm){
		(*curTerm)->ORwithDOM(val);
	}
}


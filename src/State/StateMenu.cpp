#include "StateMenu.h"
#include <iostream>

using namespace std;

StateMenu::StateMenu(){
    // constructor
    std::cout << "Estado Menu Creado" << std::endl;
}


StateMenu::~StateMenu(){
    // destructor
}


void StateMenu::InitState() {
    
}


void StateMenu::Render(){

}


void StateMenu::Update(){
    std::cout << "Actualizado estado menu" << std::endl;
}
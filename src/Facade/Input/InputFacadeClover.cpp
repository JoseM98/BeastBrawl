#include "InputFacadeClover.h"

InputFacadeClover::InputFacadeClover(){
    eventManager = EventManager::GetInstance();
    /* Dynamically load mylib.so */
    SDL_LoadObject("../../../lib/sdl/libSDL2.so");


}

InputFacadeClover::~InputFacadeClover(){

}

void InputFacadeClover::CheckInputs(float deltaTime, Entity* car, Entity* cam){
    cout << "Entro\n";
}

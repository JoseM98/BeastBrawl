#include "StatePause.h"
#include <iostream>

using namespace std;

StatePause::StatePause(){
    // constructor
    std::cout << "Estado Pause Creado" << std::endl;
    // Inicializamos las facadas
    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    //inputEngine = InputFacadeManager::GetInstance()->GetInputFacade();
    //renderFacadeManager->InitializeIrrlicht();

    renderEngine->FacadeInitPause();

    InitState();

}

void StatePause::InitState() {
    cout << "~~~ ENTRO A PAUSE" << endl;

    if (!soundEngine) {
        soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
        cout << "~~~ SoundEngine en PAUSE es -> " << soundEngine << endl;
    }
    soundEngine->PauseAllEvent();
}


void StatePause::Render(){
    renderEngine->FacadeBeginScene();
    renderEngine->FacadeDrawPause();
    renderEngine->FacadeEndScene();
}


void StatePause::Input(){
    renderEngine->FacadeCheckInputPause();
    InputFacadeManager::GetInstance()->GetInputFacade()->CheckInputPause();
}


void StatePause::Update(){
    //EventManager::GetInstance().Update();

}
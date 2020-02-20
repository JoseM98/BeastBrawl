#include "Game.h"
#include "Facade/Input/InputFacadeManager.h"
#include "Facade/Physics/PhysicsFacadeManager.h"
#include "Facade/Render/RenderFacadeManager.h"
#include "State/StateEndRace.h"
#include "State/StateInGameSingle.h"
#include "State/StateInGameMulti.h"
#include "State/StateMenu.h"
#include "State/StatePause.h"
#include "State/StateLobbyMulti.h"

using namespace std;

Game* Game::game = 0;
Game* Game::GetInstance() {
    if (game == 0) {
        game = new Game();
    }
    return game;
}

void Game::SetState(State::States stateType) {

    cout << "GAME inicia estado nuevo" << endl;

    switch (stateType) {
        case State::INTRO:
            //currentState = new StateIntro();
            break;
        case State::MENU:
            //Al volver al menu todo el mundo se desuscribe o sea que volvemos a añadir las suscripciones
            EventManager::GetInstance().ClearEvents();
            EventManager::GetInstance().ClearListeners();
            currentState = make_shared<StateMenu>();
            SuscribeEvents();
            gameStarted = false;
            break;
        case State::CONTROLS:
            //currentState = new StateControls();
            break;
        case State::CREDITS:
            //currentState = new StateCredits();
            break;
        case State::MAP:
            //currentState = new StateMap();
            break;
        case State::INGAME_SINGLE:
            if (!gameStarted) {
                currentState = make_shared<StateInGameSingle>();
                gameState = currentState;
                gameStarted = true;
            } else {
                currentState = gameState;
            }
            break;
        case State::INGAME_MULTI:
            if (!gameStarted) {
                shared_ptr<State> newState = make_shared<StateInGameMulti>();
                currentState = newState;
                gameState = currentState;
                gameStarted = true;
            } else {
                currentState = gameState;
            }
            break;
        case State::PAUSE:
            currentState = make_shared<StatePause>();
            break;
        case State::ENDRACE:
            currentState = make_shared<StateEndRace>();
            break;
        case State::LOBBY_MULTI:
            currentState = make_shared<StateLobbyMulti>();
            break;
        default:
            cout << "This state doesn't exist" << endl;
    }

    // Inicializa los bancos cada vez que se cambia de estado.
    currentState->InitState();
}

void Game::InitGame() {
    
    RenderFacadeManager::GetInstance()->InitializeIrrlicht();
    InputFacadeManager::GetInstance()->InitializeIrrlicht();
    PhysicsFacadeManager::GetInstance()->InitializeIrrlicht();

    //Inicializa la fachada de FMOD.
    SoundFacadeManager::GetInstance()->InitializeFacadeFmod();
    SoundFacadeManager::GetInstance()->GetSoundFacade()->InitSoundEngine();

    SuscribeEvents();

    cout << "Game Init" << endl;
    cout << "**********************************************" << endl;
}

void Game::SuscribeEvents(){
    cout << "Suscripciones\n";
    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::STATE_MENU,
        bind(&Game::SetStateMenu, this, placeholders::_1),
        "StateMenu"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::STATE_PAUSE,
        bind(&Game::SetStatePause, this, placeholders::_1),
        "StatePause"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::STATE_INGAMESINGLE,
        bind(&Game::SetStateInGameSingle, this, placeholders::_1),
        "StateInGameSingle"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::STATE_INGAMEMULTI,
        bind(&Game::SetStateInGameMulti, this, placeholders::_1),
        "StateInGameMulti"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::STATE_ENDRACE,
        bind(&Game::SetStateEndRace, this, placeholders::_1),
        "StateEndRace"));

    EventManager::GetInstance().SuscribeMulti(Listener(
        EventType::STATE_LOBBYMULTI,
        bind(&Game::SetStateLobbyMulti, this, placeholders::_1),
        "SetStateLobbyMulti"));

}

void Game::MainLoop() {
    SoundFacadeManager* soundFacadeManager = SoundFacadeManager::GetInstance();

    RenderFacadeManager* renderFacadeMan = RenderFacadeManager::GetInstance();
    renderFacadeMan->GetRenderFacade()->FacadeSetWindowCaption("Beast Brawl");

    while (renderFacadeMan->GetRenderFacade()->FacadeRun()) {

        
        timeElapsed = duration_cast<std::chrono::microseconds>(system_clock::now()-start).count();   
        // if(timeElapsed > updateTickTime){
        if(timeElapsed >= updateTickTime - 1000){
            cout << "////////////////// Entramos en el update /////////////////" << endl;
            start = system_clock::now();

            currentState->Input();
            currentState->Update();
            // int64_t tiempoQueSePasa = timeElapsed - updateTickTime;
            timeElapsed = duration_cast<std::chrono::microseconds>(system_clock::now()-start).count();
            cout << "El timeElapsed dentro del input es " << timeElapsed << endl;
            // cout << "El tiempoQueSePasa dentro del input es " << tiempoQueSePasa << endl;
            // timeElapsed += tiempoQueSePasa;
            
        }

        //Actualiza el motor de audio.
        soundFacadeManager->GetSoundFacade()->Update();

        cout << "TimeElapsed[" << timeElapsed << "]" << endl;
        // cout << "UpdateTickTime[" << updateTickTime << "]" << endl;

        currentState->Render(timeElapsed, updateTickTime);
    }

    renderFacadeMan->GetRenderFacade()->FacadeDeviceDrop();

    cout << "Game Main Loop" << endl;
}

void Game::TerminateGame() {
    //Libera los sonidos y bancos.
    SoundFacadeManager::GetInstance()->GetSoundFacade()->TerminateSoundEngine();
    cout << "**********************************************" << endl;
    cout << "Game Terminate" << endl;
}


//Funciones del EventManager

void Game::SetStateMenu(DataMap* d){
    cout << "LLEGA\n";
    SetState(State::MENU);
}

void Game::SetStatePause(DataMap* d){
    SetState(State::PAUSE);
}

void Game::SetStateInGameSingle(DataMap* d){
    SetState(State::INGAME_SINGLE);
}

void Game::SetStateInGameMulti(DataMap* d){
    //SetState(State::INGAME_MULTI);
    auto dataServer = any_cast<string>((*d)["dataServer"]);
    if (!gameStarted) {
        shared_ptr<State> newState;
        if(dataServer == "")
            newState = make_shared<StateInGameMulti>();
        else
            newState = make_shared<StateInGameMulti>(dataServer);
        currentState = newState;
        gameState = currentState;
        gameStarted = true;
    } else {
        currentState = gameState;
    }
}


void Game::SetStateEndRace(DataMap* d){
    SetState(State::ENDRACE);
}

void Game::SetStateLobbyMulti(DataMap* d){
    SetState(State::LOBBY_MULTI);
}
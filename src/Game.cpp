#include "Game.h"
#include "State/State.h"
#include "State/StateMenu.h"
#include "State/StateInGame.h"

using namespace std;

Game* Game::game = 0;


Game::Game(){
    // constructor
}



Game::~Game(){
    // destructor
    delete currentState;
    delete game;    // To-Do mirar donde poner este delete
}



Game* Game::GetInstance(){
    if(game == 0){
        game = new Game();
    }
    return game;
}



void Game::SetState(State::States stateType){
    switch(stateType){
        case State::INTRO:
            //currentState = new StateIntro();
            break;
        case State::MENU:
            currentState = new StateMenu();
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
        case State::INGAME:
            currentState = new StateInGame();
            break;
        case State::ENDRACE:
            //currentState = new StateEndRace();
            break;
        default:
            cout << "This state doesn't exist" << endl;
    }
}



void Game::InitGame(){
    // To-Do put window values
    MainLoop();
}



void Game::MainLoop(){
    currentState->Update();
    //for(;;);  // To-Do: crear bucle del juego
}
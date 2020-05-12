#include "StateLobbyMulti.h"
#include <iostream>

#include "../Constants.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Online/TCPClient.h"
#include "../Components/CId.h"
#include "../Components/CNavMesh.h"

using namespace std;

StateLobbyMulti::StateLobbyMulti() : tcpClient{make_shared<TCPClient>(Constants::SERVER_HOST, SERVER_PORT_TCP)} {
    std::cout << "> LOBBY constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitLobbyMulti();

    SubscribeToEvents();
}

// Cargamos los bancos de sonido Menu.
void StateLobbyMulti::InitState() {
    // if (!soundEngine){
    //     soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    // }
    // Estado a 6, no hace nada de sonido
    // soundEngine->SetState(6);
}

void StateLobbyMulti::Render() {
    renderEngine->FacadeBeginScene();

    if(actualState == StatesLobbyMulti::SELECTING_CHARACTER){
        renderEngine->FacadeDrawLobbyMultiSelChar();
        renderEngine->FacadeDrawAll();
    }else if(actualState == StatesLobbyMulti::WAITING_OTHER){
        renderEngine->FacadeDrawLobbyMultiSelChar();
        renderEngine->FacadeDrawAll();
        renderEngine->FacadeDrawLobbyMultiWait();
    }else if(actualState == StatesLobbyMulti::SERVER_FULL){
        renderEngine->FacadeDrawLobbyMultiExit();
    }else if(actualState == StatesLobbyMulti::CONNECTING){
        renderEngine->FacadeDrawLobbyMultiConnecting();
    }else if(actualState == StatesLobbyMulti::WAIT_CHARACTER_RESPONSE){
        renderEngine->FacadeDrawLobbyMultiSelChar();                    // mientras esperamos dibujamos lo mismo de antes
        renderEngine->FacadeDrawAll();
    }
        
    renderEngine->FacadeEndScene(); 
}

void StateLobbyMulti::Input() {
    if(actualState == StatesLobbyMulti::SELECTING_CHARACTER)
        renderEngine->FacadeCheckInputLobbyMultiSelChar();
    else if(actualState == StatesLobbyMulti::WAITING_OTHER)
        renderEngine->FacadeCheckInputLobbyMultiWait();
    else if(actualState == StatesLobbyMulti::SERVER_FULL)
        renderEngine->FacadeCheckInputLobbyMultiExit();
    else if(actualState == StatesLobbyMulti::CONNECTING)
        renderEngine->FacadeCheckInputLobbyMultiConnecting();
}

void StateLobbyMulti::Update() {
    EventManager::GetInstance().Update();
    //std::cout << "hola" << std::endl;

    if(actualState == StatesLobbyMulti::SERVER_FULL)
        Timer();
}


void StateLobbyMulti::SubscribeToEvents() {
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_TCP_START_MULTI,
        bind(&StateLobbyMulti::StartGameMulti, this, placeholders::_1),
        "StartGameMulti"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::PREPARE_TO_DISCONNECT,
        bind(&StateLobbyMulti::ShowDisconnection, this, placeholders::_1),
        "ShowDisconnection"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::PREPARE_TO_SELECT_CHAR,
        bind(&StateLobbyMulti::ShowSelectCharacter, this, placeholders::_1),
        "ShowSelectCharacter"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::TCP_CHAR_REQUEST,
        bind(&StateLobbyMulti::SendCharacterRequest, this, placeholders::_1),
        "SendCharacterRequest"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::TCP_WAIT_OTHERS,
        bind(&StateLobbyMulti::ChangeToWait, this, placeholders::_1),
        "ChangeToWait"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::TCP_SEL_CHAR,
        bind(&StateLobbyMulti::ChangeToSelChar, this, placeholders::_1),
        "ChangeToSelChar"));
    
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::RETURN_TO_SELCHAR,
        bind(&StateLobbyMulti::ReturnToSelChar, this, placeholders::_1),
        "ReturnToSelChar"));
}

void StateLobbyMulti::StartGameMulti(DataMap* d) {
    cout << "Hemos llegado a StateLobbyMulti::StartGameMulti" << endl;
    uint16_t idOnline = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    cout << "Yo soy el coche con idOnline " << idOnline  << endl;
    vector<uint16_t> vectorIdOnline = any_cast<vector<uint16_t>>((*d)[DataType::VECTOR_ID_ONLINE]);
    vector<uint8_t> vectorCharacters = any_cast<vector<uint8_t>>((*d)[DataType::CHARACTERS_ONLINE]);
    // renderEngine->ThrowEventChangeToMulti(idOnline, vectorIdOnline);

    //  numEnemyCars = 0;
    //Manera un poco cutre de resetear el CId al empezar el juego
    auto cId = make_shared<CId>();
    cId->ResetNumIds();
    auto cNavMesh = make_shared<CNavMesh>();
    cNavMesh->ResetNumIds();
    //Game::GetInstance()->SetState(State::INGAME_MULTI);
    shared_ptr<DataMap> data = make_shared<DataMap>();
    //(*data)[DataType::DATA_SERVER] = dataServer;
    (*data)[DataType::ID_ONLINE] = idOnline;
    (*data)[DataType::VECTOR_ID_ONLINE] = vectorIdOnline;
    (*data)[DataType::CHARACTERS_ONLINE] = vectorCharacters;
    EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_INGAMEMULTI, data});
}


void StateLobbyMulti::ShowDisconnection(DataMap* d) {
    actualState = StatesLobbyMulti::SERVER_FULL;
}

void StateLobbyMulti::ShowSelectCharacter(DataMap* d) {
    actualState = StatesLobbyMulti::SELECTING_CHARACTER;
}

void StateLobbyMulti::SendCharacterRequest(DataMap* d) {
    actualState = StatesLobbyMulti::WAIT_CHARACTER_RESPONSE;
    tcpClient->SendSelCharacterRequest();
}

void StateLobbyMulti::ChangeToWait(DataMap* d){
    actualState = StatesLobbyMulti::WAITING_OTHER;
}

void StateLobbyMulti::ChangeToSelChar(DataMap* d){
    actualState = StatesLobbyMulti::SELECTING_CHARACTER;
}

void StateLobbyMulti::ReturnToSelChar(DataMap* d){
    tcpClient->SendCancelChar();
    actualState = StatesLobbyMulti::SELECTING_CHARACTER;
}



void StateLobbyMulti::Timer(){
    if(valueTimer<valueMaxTimer)
        valueTimer++;
    else
        EventManager::GetInstance().AddEventMulti(Event{EventType::STATE_MENU});
    
}
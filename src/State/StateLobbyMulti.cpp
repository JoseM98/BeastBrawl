#include "StateLobbyMulti.h"
#include <iostream>

#include "../Constants.h"
#include "../EventManager/Event.h"
#include "../EventManager/EventManager.h"
#include "../Online/TCPClient.h"

using namespace std;

StateLobbyMulti::StateLobbyMulti() : tcpClient{make_shared<TCPClient>(Constants::SERVER_HOST, SERVER_PORT_TCP)} {
    std::cout << "> LobbyMenu constructor" << std::endl;

    renderEngine = RenderFacadeManager::GetInstance()->GetRenderFacade();
    renderEngine->FacadeInitLobbyMulti();

    SubscribeToEvents();

    cout << "Enviamos SendConnectionRequest al server" << endl;
    tcpClient->SendConnectionRequest();
    //tcpClient->SendDateTime();
}

// Cargamos los bancos de sonido Menu.
void StateLobbyMulti::InitState() {
    // cout << "~~~ ENTRO A MENU" << endl;
    // if (!soundEngine){
    //     soundEngine = SoundFacadeManager::GetInstance()->GetSoundFacade();
    //     cout << "~~~ SoundEngine en MENU es -> " << soundEngine << endl;
    // }
    // soundEngine->SetState(2);
    // soundEngine->PlayEvent("Musica/menu");

    //cout << "> MENU iniciado" << endl;
}

void StateLobbyMulti::Render() {
    renderEngine->FacadeDrawLobbyMulti();
}

void StateLobbyMulti::Input() {
    renderEngine->FacadeCheckInputLobbyMulti();
}

void StateLobbyMulti::Update() {
    EventManager::GetInstance().Update();
    //std::cout << "hola" << std::endl;
    SendData();
}

void StateLobbyMulti::SendData() {
    //tcpClient->SendDateTime();
}

void StateLobbyMulti::SubscribeToEvents() {
    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_TCP_START_MULTI,
        bind(&StateLobbyMulti::StartGameMulti, this, placeholders::_1),
        "StartGameMulti"));

    EventManager::GetInstance().SubscribeMulti(Listener(
        EventType::NEW_TCP_RETURN_MENU,
        bind(&StateLobbyMulti::SendDisconnectionMenu, this, placeholders::_1),
        "SendDisconnectionMenu"));
}

void StateLobbyMulti::StartGameMulti(DataMap* d) {
    cout << "Hemos llegado a StateLobbyMulti::StartGameMulti" << endl;
    uint16_t idOnline = any_cast<uint16_t>((*d)[DataType::ID_ONLINE]);
    cout << "Yo soy el coche con idOnline " << idOnline  << endl;
    vector<uint16_t> vectorIdOnline = any_cast<vector<uint16_t>>((*d)[DataType::VECTOR_ID_ONLINE]);
    renderEngine->ThrowEventChangeToMulti(idOnline, vectorIdOnline);
}


void StateLobbyMulti::SendDisconnectionMenu(DataMap* d){
    tcpClient->SendDisconnectionRequest();
}

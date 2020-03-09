#pragma once

#include "../../include/boost/array.hpp"
#include "../../include/boost/asio.hpp"
#include "../../include/boost/enable_shared_from_this.hpp"
#include <chrono>
#include <iostream>
#include "../src/Constants.h"
#include "Player.h"

using boost::asio::ip::udp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class UDPServer : public boost::enable_shared_from_this<UDPServer> {
   public:
    UDPServer(asio::io_context& context_, uint16_t port_);
    ~UDPServer();
    void StartReceiving();
    void Close();
    void CheckDisconnectionsAfterSeconds();
    void ResetTimerStartReceiving();
   private:
    void SavePlayerIfNotExists(const uint16_t id, udp::endpoint& endpoint);

    void HandleReceive(std::shared_ptr<unsigned char[]> recevBuff, std::shared_ptr<udp::endpoint> remoteEndpoint, const boost::system::error_code& error, size_t bytesTransferred);
    // void HandleReceive(const boost::system::error_code& error, size_t bytesTransferred);
    void HandleReceivedInputs(const uint16_t id, const unsigned char resendInputs[], const size_t currentBufferSize, const udp::endpoint& remoteClient);
    void HandleReceivedSync(const uint16_t id, unsigned char resendSync[], const size_t currentBufferSize, const udp::endpoint& remoteClient);
    void HandleReceivedCatchPU(const uint16_t id, unsigned char resendPU[], const size_t currentBufferSize, const udp::endpoint& remoteClient);
    void HandleReceivedCatchTotem(const uint16_t id, unsigned char buffer[], const size_t currentBufferSize, const udp::endpoint& remoteClient);
    void HandleReceivedLostTotem(const uint16_t id, unsigned char buffer[], const size_t currentBufferSize, const udp::endpoint& remoteClient);
    void HandleReceivedUsedRoboJorobo(const uint16_t id, unsigned char buffer[], const size_t currentBufferSize, const udp::endpoint& remoteClient);
    void HandleReceivedCollideNitro(const uint16_t id, unsigned char buffer[], const size_t currentBufferSize, const udp::endpoint& remoteClient);
    void HandleReceivedThrowPU(const uint16_t id, const uint16_t idPUOnline, uint8_t typePU, unsigned char buffer[], const size_t currentBufferSize, const udp::endpoint& remoteClient);
    
    void ResendBytesToOthers(const uint16_t id, const unsigned char resendBytes[], const size_t currentBufferSize, const udp::endpoint& remoteClient);
    void SendBytes(const unsigned char resendBytes[], const size_t currentBufferSize, const Player& player);
    //void SendInputs(const vector<Constants::InputTypes> inputs, const Player& player);
    void HandleSentBytes(const boost::system::error_code& errorCode, std::size_t bytesTransferred) const;
    void DetectUsersDisconnected();

    void ReceiveNewCar();
    void SendEndgame(const Player &p);

    void RequestId();
    void Exit();
    Player* GetPlayerById(uint16_t id);

    // --- TCP --- (sala de espera)
    // el cliente por tcp le dice al server que se quiere conectar
    // el server le devuelve un id
    // pedir otros jugadores

    //ejemplo
    // server recibe un mensaje tcp de 1 cliente
    //      -> el server lo guarda en el array de clientes, ya hay 1
    // server recibe un mensaje tcp de 1 cliente
    //      -> el server lo guarda en el array de clientes, ya hay 2
    // server recibe un mensaje tcp de 1 cliente
    //      -> el server lo guarda en el array de clientes, ya hay 3
    // server recibe un mensaje tcp de 1 cliente
    //      -> el server lo guarda en el array de clientes, ya hay 4
    // el servidor envía a todos los clientes un mensaje de inicio de partida
    //      en el que se incluye el estado inicial del juego, con el orden
    //      (aleatorio o no) de los spawns. Cada cliente entonces sabe qué
    //      num de spawn le toca y por tanto sabe dónde se tiene que
    //      posicionar al inicio

    // --- UDP --- (juego)
    // todos los clientes empiezan la partida, y en el primera paquete
    // udp, se actualizará el endpoint udp que corresponda a cada cliente

    asio::io_context& context;
    udp::socket socket;
    std::vector<Player> players;
    std::vector<uint16_t> idsMelons;
    std::vector<uint16_t> idsBananas;
    std::vector<uint16_t> idsPuddings;

    const uint16_t SEGUNDOS = 1000;
    const uint32_t TIEMPO_DESCONEXION = 5 * SEGUNDOS;
    const uint8_t TIME_BETWEEN_DISCONNECTION_CHECKS = 2;

    std::unique_ptr<boost::asio::steady_timer> timer;
    int64_t timeServerStartedReceiving;

    uint16_t playerWithTotem = Constants::ANY_PLAYER;

    const uint8_t NUM_REINTENTOS = 3;
};

#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <chrono>
#include <iostream>

using boost::asio::ip::udp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class UDPServer : public boost::enable_shared_from_this<UDPServer> {
   public:
    UDPServer(asio::io_context& context_, uint16_t port_);
    void StartReceiving();

   private:

    void SaveClientIfNotExists(udp::endpoint& endpoint);

    void HandleReceive(std::shared_ptr<boost::array<char, 1024>> recevBuff, std::shared_ptr<udp::endpoint> remoteEndpoint, const boost::system::error_code& error, size_t bytesTransferred);
    // void HandleReceive(const boost::system::error_code& error, size_t bytesTransferred);
    void HandleReceiveInput(const uint16_t input, const udp::endpoint& remoteClient);
    void HandleReceiveDateTimeRequest(const udp::endpoint& remoteClient);
    void HandleSentDateTimeRequest(const boost::shared_ptr<string> message,
                                   const boost::system::error_code& errorCode,
                                   std::size_t bytes_transferred);
    void HandleSentDefaultMessage(const boost::shared_ptr<string> message,
                                  const boost::system::error_code& errorCode,
                                  std::size_t bytes_transferred);
    void ResendInputToOthers(const uint16_t inputType, const udp::endpoint& remoteClient);
    
    void ReceiveNewCar();

    void RequestId();
    
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

    string GetTime() {
        auto time_point = system_clock::now();
        time_t now_c = system_clock::to_time_t(time_point);
        string salida = ctime(&now_c);
        salida = salida.substr(0, salida.size() - 1);
        return salida;
    }

    

    udp::socket socket;
    udp::endpoint receiverEndpoint;
    std::vector<udp::endpoint> clients;
    boost::array<char, 512> recvBuff;
    asio::streambuf inPacket;
};
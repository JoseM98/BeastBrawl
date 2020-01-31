#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <chrono>
#include <iostream>
#include <thread>
#include "../../include/include_json/include_json.hpp"
#include "../Constants.h"

using boost::asio::ip::udp;
using namespace std;
using namespace std::chrono;
using json = nlohmann::json;

class UDPClient {
    // --- TCP --- (sala de espera)
    // .- enviamos al servidor un mensaje de que queremos conectar
    // .- esperamos hasta recibir respuesta de que empieza la partida.
    //      Este mensaje traerá información de el estado inicial de la partida
    //      con el numero de spawn del todos los coches, de esta forma ya sabemos
    //      dónde debemos posicionar todos los coches de inicio

    // --- UDP --- (juego)
    // .- cambiamos de estado a StateInGameMulti
    // .- inicializamos la partida con la info que nos ha llegado del server
    // .- en versión futura se puede recibir una hora de inicio y
    //       ese es el momento en el que empieza
    // .- Empieza la partida, y le enviaremos paquetes de input al servidor y
    //      él se encargará de registrar nuestro endpoint udp

   public:
    UDPClient(string host, string port_);
    ~UDPClient();

    void SendInput(Constants::InputTypes input);
    void SendInputs(vector<Constants::InputTypes>& inputs, uint32_t id);
    void SendDateTime();
    uint32_t idMainCar;

   private:
    void StartReceiving();
    void HandleReceived(std::shared_ptr<boost::array<char, 1024>> recvBuff, const boost::system::error_code& error, size_t bytesTransferred);
    void HandleReceivedInput(const json, const uint32_t id) const;
    void HandleSentInput(std::shared_ptr<Constants::InputTypes> input, const boost::system::error_code& errorCode,
                         std::size_t bytes_transferred);
    void HandleSentInputs(const boost::system::error_code& errorCode,
                          std::size_t bytes_transferred);
    void HandleSentDateTime(const boost::shared_ptr<std::string> message,
                            const boost::system::error_code& errorCode,
                            std::size_t bytes_transferred);

    boost::asio::io_context context;
    udp::endpoint serverEndpoint;
    udp::socket socket;
    std::thread butler;
    // boost::asio::io_context::strand strand;
};
#pragma once

#include "../../include/boost/array.hpp"
#include "../../include/boost/asio.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include "TCPConnection.h"
#include "Player.h"

using boost::asio::ip::tcp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class UDPServer;

class TCPServer{
   public:
    TCPServer(boost::asio::io_context& context_, uint16_t port_, UDPServer &udpServer_);
    ~TCPServer();
    void StartReceiving();
    void Close();
    void SendStartGame();

   private:
    
    void HandleAccept(std::shared_ptr<TCPConnection> new_connection, const boost::system::error_code& error);
    bool PlayerExists(std::shared_ptr<TCPConnection> new_connection);

    boost::asio::io_context& context;
    tcp::acceptor acceptor_;
    UDPServer &udpServer;

    vector<std::shared_ptr<TCPConnection>> connections;
    std::vector<Player> players;
    std::vector<uint8_t> characters;
};
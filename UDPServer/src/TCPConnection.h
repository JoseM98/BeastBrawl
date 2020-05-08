#pragma once

#include "../../include/boost/asio.hpp"
#include <memory>

#include <chrono>
#include <iostream>
#include "Player.h"

using boost::asio::ip::tcp;
using namespace boost;
using namespace std;
using namespace std::chrono;

class TCPServer;

class TCPConnection {
    public:
    TCPConnection(TCPServer *tcpServer_, asio::io_context& io_context, std::vector<Player> &p, std::vector<uint8_t> &c, std::vector<std::shared_ptr<TCPConnection>>& connect);
    ~TCPConnection();
    tcp::socket& socket(){ return socket_;}
    void Start();
    void Close();
    void SendStartMessage(string datos);
    void SendStartMessage(unsigned char *buff, size_t buffSize);
    void SendFullGame();
    // Player *currentPlayer;


   private:
    void HandleRead(std::shared_ptr<unsigned char[]> recevBuff, const boost::system::error_code& error, size_t bytes_transferred);
    void HandleWrite(const boost::system::error_code& error, size_t bytes_transferred);
    void DeleteMe();

    string GetTime() {
        auto time_point = system_clock::now();
        time_t now_c = system_clock::to_time_t(time_point);
        string salida = ctime(&now_c);
        salida = salida.substr(0, salida.size() - 1);
        return salida;
    }

    TCPServer *tcpServer;
    tcp::socket socket_;

    std::vector<Player> &players;
    std::vector<uint8_t> &characters;
    std::vector<std::shared_ptr<TCPConnection>>& connections;

    //uint16_t sendBuff;

};
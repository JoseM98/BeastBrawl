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

class TCPConnection : public std::enable_shared_from_this<TCPConnection> {
    public:
    ~TCPConnection();
    typedef std::shared_ptr<TCPConnection> pointer;
    static pointer Create(boost::asio::io_context& io_context, std::vector<Player> &p, std::vector<TCPConnection::pointer>& connect){ return pointer(new TCPConnection(io_context, p, connect)); }
    tcp::socket& socket(){ return socket_;}
    void Start();
    void Close();
    void SendStartMessage(string datos);
    void SendStartMessage(unsigned char *buff, size_t buffSize);
    void SendFullGame();


   private:
    TCPConnection(asio::io_context& io_context, std::vector<Player> &p, std::vector<TCPConnection::pointer>& connect);
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

    tcp::socket socket_;

    std::vector<Player> &players;
    std::vector<TCPConnection::pointer>& connections;

    //uint16_t sendBuff;

};
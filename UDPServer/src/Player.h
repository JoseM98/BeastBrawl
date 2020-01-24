#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

struct Player {
   public:
    Player() = default;
    ~Player() = default;
    uint32_t id{nextId++};
    udp::endpoint endpoint;
    tcp::endpoint endpointTCP;

   private:
    static uint32_t nextId;
};
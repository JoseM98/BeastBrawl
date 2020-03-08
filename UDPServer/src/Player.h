#pragma once

#include <iostream>
#include "../../include/boost/asio.hpp"

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

struct Player {
   public:
    Player() = default;
    ~Player() = default;
    uint16_t id{nextId++};
    udp::endpoint endpoint;
    tcp::endpoint endpointTCP;
    int64_t lastInputTimeReceived{0};
    int64_t lastSyncTimeReceived{0};
    int64_t lastCatchPUTimeReceived{0};
    int64_t lastCatchTotemTimeReceived{0};

    bool hasTotem {false};
    bool disconnected {false};
    bool readyToDelete {false};

    static uint16_t nextId;
   private:
};
#pragma once

#include <iostream>
#include "../../include/boost/asio.hpp"
//#include "TCPConnection.h"

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

struct Player {
   public:
    Player() = default;
    ~Player() = default;
    void setId(uint16_t _id);

    uint16_t id{nextId++};
    udp::endpoint endpoint;
    tcp::endpoint endpointTCP;
    //TCPConnection::pointer connectionPointer;
    int64_t lastInputTimeReceived{0};
    int64_t lastSyncTimeReceived{0};
    int64_t lastCatchPUTimeReceived{0};
    int64_t lastCatchTotemTimeReceived{0};
    int64_t lastLostTotemTimeReceived{0};
    int64_t lastUsedRoboJoroboTimeReceived{0};
    int64_t lastCollideNitroTimeReceived{0};
    int64_t lastThrowPUReceived{0};
    int64_t lastCrashPUCarReceived{0};
    int64_t lastCrashPUWallReceived{0};
    int64_t lastLaunchAnimationEndReceived{0};
    int64_t lastWaitingForCountdownReceived{0};

    bool hasTotem {false};
    bool disconnected {false};
    bool readyToDelete {false};
    bool waitingForCountdown {false};


    static uint16_t nextId;
   private:
};
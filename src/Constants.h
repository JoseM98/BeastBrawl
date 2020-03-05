#pragma once

#include <cmath>
#include <cstdint>
#include <cstddef>

#define SERVER_HOST "localhost"
#define SERVER_PORT_UDP 1234
#define SERVER_PORT_TCP 1235
#define UPDATE_FRAME_RATE 20.0
#define FRAME_RATE 60.0

#define SERVER_HOST_OVH "54.38.189.168"

#define MIN_NUM_PLAYERS 2


class Constants {
   public:
    inline static const uint16_t TIME_BETWEEN_UPDATES_ms{uint16_t(1.0 / UPDATE_FRAME_RATE * 1000)};
    inline static const double TIME_BETWEEN_UPDATES_us{TIME_BETWEEN_UPDATES_ms * 1000};
    inline static const double DELTA_TIME{1.0 / UPDATE_FRAME_RATE};
    inline static const bool DEBUG_SHOW_CHASSIS{true};
    inline static const bool DEBUG_SHOW_SPHERES{true};
    inline static const size_t ONLINE_BUFFER_SIZE{512};
    

    enum InputTypes { FORWARD,
                      BACK,
                      LEFT,
                      RIGHT,
                      LAUNCH_PU,
                      CLAXON,
                      DRIFT };

    enum PetitionTypes { CONNECTION_REQUEST,
                         SEND_INPUTS,
                         SEND_SYNC };
};
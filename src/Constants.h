#pragma once

#define SERVER_HOST "localhost"
#define SERVER_PORT_UDP "1234"
#define SERVER_PORT_TCP "1235"
#define UPDATE_FRAME_RATE 15.0
#define FRAME_RATE 60.0


class Constants {
   public:
    static const bool DEBUG_SHOW_SPHERES{true};

    enum InputTypes { FORWARD,
                      BACK,
                      LEFT,
                      RIGHT,
                      LAUNCH_PU,
                      CLAXON };

    enum PetitionTypes { SEND_INPUT,
                         SEND_INPUTS,
                         SEND_SYNC };
};
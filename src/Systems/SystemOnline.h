#pragma once

#include <iostream>
#include "../Aliases.h"
#include "../Constants.h"
#include "../Online/UDPClient.h"
#include "../Components/CPowerUp.h"
using namespace std;

class ManCar;
class CPowerUp;
class ManTotem;

class SystemOnline {
   public:
    SystemOnline(ManCar &manCar_, uint16_t idOnlineMainCar_);
    ~SystemOnline(){};

    void Update();
    void SendInputs(const vector<Constants::InputTypes> &d);
    void SendSync(ManCar* manCars, ManTotem* manTotem);
    void SendCatchPU(CPowerUp& cPowerUp);
    uint16_t idOnlineMainCar;

   private:
    ManCar &manCar;
    unique_ptr<UDPClient> udpClient;
};
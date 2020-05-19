#include "CNitro.h"

CNitro::CNitro(){
    m_compType = CompType::NitroComp;
    activePowerUp = false;
}

CNitro::~CNitro(){
    
}


void CNitro::activatePowerUp(){
    activePowerUp = true;
    timeStart = system_clock::now();
    nitroAcceleration = 50.0; 
    nitroMaxSpeed = 400.0; 
}


void CNitro::deactivePowerUp(){
    activePowerUp = false;
    nitroAcceleration = 0;
    nitroMaxSpeed = 0;
}
#pragma once

#include <iostream>
#include <map>

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "../../common.h"

#include "SoundFacade.h"

class SoundFacadeFMOD : public SoundFacade {
    public:
        explicit SoundFacadeFMOD() : SoundFacade() {};
        virtual ~SoundFacadeFMOD() {};

        virtual void initSoundEngine();
        virtual void terminateSoundEngine();
        virtual void loadMasterBank();
        virtual void unloadMasterBank();
        virtual void addInstanceSound(const char*);

        virtual void loadBanks();
        virtual void unloadBanks();

        virtual void update();

    private:
        FMOD::System* coreSystem = NULL;
        FMOD::Studio::System* system = NULL;
        FMOD::Studio::Bank* masterBank = NULL;
        FMOD::Studio::Bank* stringsBank = NULL;

        std::map<std::string, FMOD::Studio::EventInstance*> instances;
};
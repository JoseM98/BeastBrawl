#pragma once

#include <iostream>
#include <unordered_map>

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "../../common.h"

#include "SoundFacade.h"

class SoundFacadeFMOD : public SoundFacade {
    public:
        explicit SoundFacadeFMOD() : SoundFacade() {};
        virtual ~SoundFacadeFMOD();

        virtual void InitSoundEngine();
        virtual void TerminateSoundEngine();
        virtual void LoadMasterBank();
        virtual void UnloadMasterBank();
        virtual void AddInstanceSound(const char*);

        virtual void LoadBanks();
        virtual void UnloadBanks();

        virtual void Update();

    private:
        FMOD::System* coreSystem = NULL;
        FMOD::Studio::System* system = NULL;
        FMOD::Studio::Bank* masterBank = NULL;
        FMOD::Studio::Bank* stringsBank = NULL;

        std::unordered_map<std::string, FMOD::Studio::EventInstance*> instances;
};
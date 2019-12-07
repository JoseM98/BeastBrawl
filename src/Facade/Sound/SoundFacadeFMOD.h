#pragma once

#include <iostream>
#include <unordered_map>

#include <fmod_studio.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>

#include "../../EventManager/EventManager.h"
#include "../../EventManager/Event.h"
#include "SoundFacade.h"

class SoundFacadeFMOD : public SoundFacade {
    public:
        explicit SoundFacadeFMOD() : SoundFacade() {};
        ~SoundFacadeFMOD();

        void InitSoundEngine() override;
        void TerminateSoundEngine() override;
        void LoadMasterBank();
        void UnloadMasterBank();
        void AddInstanceSound(const char*) override;

        void LoadBanks() override;
        void UnloadBanks() override;

        void Update() override;

        std::unordered_map<std::string, FMOD::Studio::EventInstance*> GetInstances() { return instances;};


    private:
        FMOD::System* coreSystem = NULL;
        FMOD::Studio::System* system = NULL;
        FMOD::Studio::Bank* masterBank = NULL;
        FMOD::Studio::Bank* stringsBank = NULL;

        std::unordered_map<std::string, FMOD::Studio::EventInstance*> instances;
        EventManager* eventManager;
};
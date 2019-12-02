#pragma once

class SoundFacade {
    public:
        explicit SoundFacade() = default;
        
        virtual ~SoundFacade() = default;

        virtual void InitSoundEngine() = 0;
        virtual void TerminateSoundEngine() = 0;

        virtual void LoadMasterBank() = 0;
        virtual void UnloadMasterBank() = 0;

        virtual void AddInstanceSound(const char*) = 0;

        virtual void LoadBanks() = 0;
        virtual void UnloadBanks() = 0;

        virtual void Update() = 0;

    private:
    
};
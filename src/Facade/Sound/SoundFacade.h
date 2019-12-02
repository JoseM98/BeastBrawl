#pragma once

class SoundFacade {
    public:
        explicit SoundFacade() = default;
        
        virtual ~SoundFacade() = default;

        virtual void initSoundEngine() = 0;
        virtual void terminateSoundEngine() = 0;

        virtual void loadMasterBank() = 0;
        virtual void unloadMasterBank() = 0;

        virtual void addInstanceSound(const char*) = 0;

        virtual void loadBanks() = 0;
        virtual void unloadBanks() = 0;

        virtual void update() = 0;

    private:
    
};
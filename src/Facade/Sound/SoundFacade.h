#pragma once

using namespace std;

class SoundFacade {
    public:
        explicit SoundFacade() = default;
        virtual ~SoundFacade() = default;

        virtual void InitSoundEngine() = 0;
        virtual void TerminateSoundEngine() = 0;

        virtual void LoadBank(const uint16_t) = 0;
        virtual void LoadBankInGame() = 0;
        virtual void LoadEvent(const char*) = 0;
        virtual void UnloadBank(const char*) = 0;

        virtual void Update() = 0;

    private:
    
};
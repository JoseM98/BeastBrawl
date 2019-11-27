#ifndef SOUNDFACADEMANAGER_H
#define SOUNDFACADEMANAGER_H

#pragma once

class SoundFacadeManager{
    public:
        SoundFacadeManager() /*: soundFacade {nullptr}*/ {}
        ~SoundFacadeManager(){ /*delete soundFacade;*/};

        void InitializeFacadeFmod() {
            //soundFacade = new SoundFacadeFmod();
        }

        //SoundFacade *GetSoundFacade() { return soundFacade; };
        //static SoundManager& GetInstance();

    private:
        //SoundFacade *soundFacade;
};

#endif // SOUNDFACADEMANAGER_H
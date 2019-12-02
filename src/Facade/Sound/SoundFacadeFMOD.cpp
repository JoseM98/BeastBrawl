#include <iostream>

#include "SoundFacadeFMOD.h"
#include "fmod_errors.h"

/*
 * FMOD ERRORS
 */

//#define ERRCHECK(_result) ERRCHECK_fn(_result,__FILE__,__LINE__)
//
//void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line) {
//    if (result != FMOD_OK) {
//        std::cerr << file << "(" << line << "): FMOD error " << result << " - " << FMOD_ErrorString(result) << std::endl;
//        exit(-1);
//    }
//}

/*
 * SOUND FACADE FMOD
 */

SoundFacadeFMOD::~SoundFacadeFMOD() {
    delete coreSystem;
    delete system;
    delete masterBank;
    delete stringsBank;
}

void SoundFacadeFMOD::InitSoundEngine() {

    void *extraDriverData = NULL;
    Common_Init(&extraDriverData);
    
    ERRCHECK( FMOD::Studio::System::create(&system) );

    ERRCHECK( system->getCoreSystem(&coreSystem) );
    ERRCHECK( coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0) );

    //Inicializa FMOD Studio, inicializando tambien FMOD Core.
    ERRCHECK( system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0) );

    LoadMasterBank();
}

void SoundFacadeFMOD::TerminateSoundEngine() {

    UnloadMasterBank();

    ERRCHECK( system->release() );
    Common_Close();
}

//TO-DO: Quitar el dichoso Common_MediaPath.
void SoundFacadeFMOD::LoadMasterBank() {
    ERRCHECK( system->loadBankFile(Common_MediaPath("Master.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );
    ERRCHECK( system->loadBankFile(Common_MediaPath("Master.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );
}

void SoundFacadeFMOD::UnloadMasterBank() {
    ERRCHECK( stringsBank->unload() );
    ERRCHECK( masterBank->unload() );
}

void SoundFacadeFMOD::AddInstanceSound(const char* nameEvent) {
    FMOD::Studio::EventDescription* musicDescription = NULL;
    ERRCHECK( system->getEvent(nameEvent, &musicDescription) );
  
    FMOD::Studio::EventInstance* musicInstance = NULL;
    ERRCHECK( musicDescription->createInstance(&musicInstance) );

    instances[nameEvent] = musicInstance;
}

void SoundFacadeFMOD::LoadBanks() {
    
}

void SoundFacadeFMOD::UnloadBanks() {
    
}

void SoundFacadeFMOD::Update() {
    Common_Update();

    if (Common_BtnPress(BTN_ACTION1)) {
        ERRCHECK( instances["event:/Ej2"]->start() );
    }
    if (Common_BtnPress(BTN_ACTION2)) {
        ERRCHECK( instances["event:/Ej2"]->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
    }

    ERRCHECK( system->update() );
}
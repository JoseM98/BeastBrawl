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

void SoundFacadeFMOD::initSoundEngine() {

    void *extraDriverData = NULL;
    Common_Init(&extraDriverData);
    
    ERRCHECK( FMOD::Studio::System::create(&system) );

    ERRCHECK( system->getCoreSystem(&coreSystem) );
    ERRCHECK( coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0) );

    //Inicializa FMOD Studio, inicializando tambien FMOD Core.
    ERRCHECK( system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0) );

    loadMasterBank();
}

void SoundFacadeFMOD::terminateSoundEngine() {

    unloadMasterBank();

    ERRCHECK( system->release() );
    Common_Close();
}

void SoundFacadeFMOD::loadMasterBank() {
    ERRCHECK( system->loadBankFile("../../../media/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );
    ERRCHECK( system->loadBankFile(Common_MediaPath("../../../media/fmod/Master.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );
}

void SoundFacadeFMOD::unloadMasterBank() {
    ERRCHECK( stringsBank->unload() );
    ERRCHECK( masterBank->unload() );
}

void SoundFacadeFMOD::loadBanks() {
    
}

void SoundFacadeFMOD::unloadBanks() {
    
}

void SoundFacadeFMOD::update() {

}
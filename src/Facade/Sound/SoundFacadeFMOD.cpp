#include <iostream>

#include "SoundFacadeFMOD.h"
#include "SoundFacadeManager.h"


void PruebaSonido(Data d);
/*
 * FMOD ERRORS
 */

#define ERRCHECK(_result) ERRCHECK_fn(_result,__FILE__,__LINE__)

void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line) {
    if (result != FMOD_OK) {
        std::cerr << file << "(Linea: " << line << "): Se ha producido un error de FMOD " << result << " - " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }
}

/*
 * SOUND FACADE FMOD
 */

SoundFacadeFMOD::~SoundFacadeFMOD() {
    delete coreSystem;
    delete system;
    delete masterBank;
    delete stringsBank;
}


//Puntero a funcion
void PruebaSonido(Data d){
    SoundFacadeManager* soundFacadeManager = SoundFacadeManager::GetInstance();
    SoundFacadeFMOD* soundEngine = static_cast<SoundFacadeFMOD*>(soundFacadeManager->GetSoundFacade());
    auto instances = soundEngine->GetInstances();
    cout << "HA ENTRADO MUSICA\n";
    ERRCHECK( instances["event:/Ej2"]->start() );

}

void SoundFacadeFMOD::InitSoundEngine() {

    eventManager = EventManager::GetInstance();
    eventManager->Suscribe(Listener {EventType::PRESS_1,PruebaSonido,"pruebaSonido"});
    
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
}

//TO-DO: Quitar el dichoso Common_MediaPath.
void SoundFacadeFMOD::LoadMasterBank() {
    ERRCHECK( system->loadBankFile("./media/fmod/Master.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );
    ERRCHECK( system->loadBankFile("./media/fmod/Master.strings.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );
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

    ERRCHECK( system->update() );
}
#include <iostream>
#include <string.h>

#include "SoundFacadeFMOD.h"
#include "SoundFacadeManager.h"

using namespace std;

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
    auto instances = soundEngine->GetDescription();
//
    if (instances.find("choque_enemigo") != instances.end()) {
        FMOD::Studio::EventInstance* instance = nullptr;
        ERRCHECK( instances["choque_enemigo"]->createInstance(&instance) );
        ERRCHECK( instance->start() );
        ERRCHECK( instance->release() );
    }
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

    soundDescriptions.clear();
    
    unordered_map<string, FMOD::Studio::Bank*>::iterator it = banks.begin();
    while(it != banks.end()) {
        it->second->unload();
        it++;
    }
    banks.clear();

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


//TO-DO: Cambio de tipo de parametro
void SoundFacadeFMOD::LoadBank(const uint16_t numBank) {
    //TO-DO: Esto se puede optimizar
    switch (numBank) {
        case 0:
            //LoadBankMenu();
            break;
        case 1:
            //Load...
            break;
        case 2:
            LoadBankInGame();
            break;
        default:
            cout << "It does not exist bank: " << numBank << endl;
    }
}

void SoundFacadeFMOD::LoadBankInGame() {
    if (banks.find("InGame") == banks.end()) {
        banks["InGame"] = nullptr;
        ERRCHECK(system->loadBankFile("./media/fmod/InGame.bank", FMOD_STUDIO_LOAD_BANK_NORMAL, &banks["InGame"]));
    }
    cout << "***** Voy a intentar cargar los eventos" << endl;
    LoadEvent("choque_enemigo");
    LoadEvent("lanzo_powerup");
}

void SoundFacadeFMOD::LoadEvent(const char* nameEvent) {
    FMOD::Studio::EventDescription* description = NULL;
    if (soundDescriptions.find(nameEvent) == soundDescriptions.end()) {
        soundDescriptions[nameEvent] = nullptr;
    }
    //TO-DO: Mejorar
    char* charEvent = "event:/";
    char* event = (char *) malloc(1 + strlen(charEvent) + strlen(nameEvent) );
    strcpy(event, charEvent);
    strcat(event, nameEvent);
    ERRCHECK( system->getEvent(event, &soundDescriptions[nameEvent]) );
    cout << "Cargado el evento: " << event << endl;
}

void SoundFacadeFMOD::UnloadBank(const char*) {
    
}

void SoundFacadeFMOD::Update() {
    ERRCHECK( system->update() );
}
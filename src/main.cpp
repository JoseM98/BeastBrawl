#include "Game.h"

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"

int main()
{

  float progreso = 0;

  void *extraDriverData = NULL;
  Common_Init(&extraDriverData);

  FMOD::Studio::System* system = NULL;
  ERRCHECK( FMOD::Studio::System::create(&system) );

  // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
  FMOD::System* coreSystem = NULL;
  ERRCHECK( system->getCoreSystem(&coreSystem) );
  ERRCHECK( coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0) );
  ERRCHECK( system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0) );
  
  FMOD::Studio::Bank* masterBank = NULL;
  ERRCHECK( system->loadBankFile(Common_MediaPath("Master.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );
  
  FMOD::Studio::Bank* stringsBank = NULL;
  ERRCHECK( system->loadBankFile(Common_MediaPath("Master.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );
  
  FMOD::Studio::EventDescription* musicDescription = NULL;
  ERRCHECK( system->getEvent("event:/Ej2", &musicDescription) );
  
  FMOD::Studio::EventInstance* musicInstance = NULL;
  ERRCHECK( musicDescription->createInstance(&musicInstance) );

  do
  {
      Common_Update();

      if (Common_BtnPress(BTN_ACTION1))
      {
        ERRCHECK( musicInstance->start() );
      }

      if (Common_BtnPress(BTN_ACTION2)) {
        if (progreso < 5) {
            progreso ++;
        }
        else {
            progreso = 0;
        }

        musicInstance->setParameterByName("progreso", progreso);
        std::cout << "Progreso: " << progreso << std::endl;
      }

      ERRCHECK( system->update() );

      Common_Sleep(50);

  } while (!Common_BtnPress(BTN_QUIT));
  
  ERRCHECK( stringsBank->unload() );
  ERRCHECK( masterBank->unload() );

  ERRCHECK( system->release() );

  Common_Close();

  return 0;

//    Game *game = Game::GetInstance();
//    
//    game->SetState(State::States::INGAME);
//    game->InitGame();
//	
//	return 0;
}
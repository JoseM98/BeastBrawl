
#include "Entities/Hero.h"
#include "Entities/GameObject.h"
#include "Components/CPosition.h"
#include "Components/CSpeed.h"
#include "Components/CId.h"
#include "Components/CType.h"
#include "Components/CTexture.h"
#include "Components/CMesh.h"
#include "Components/CTransformable.h"
#include "Systems/Physics.h"
#include "EventManager/EventManager.h"
#include "Facade/RenderFacadeManager.h"
//#include "Facade/InputFacadeManager.h"

#include "Game.h"

#include <iostream>
#include <list>
#include <string>
#include <cstdint>
#include <memory>

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"


void pruebaEvent1(Data d){
    
    std::cout << "Soy pruebaEvent1"<< "\n";
}
void pruebaEvent2(Data d){
    std::cout << "Soy pruebaEvent2"<< "\n";
}


int main()
{
  float nivel = 0;
  float hora = 0;
  float progreso = 0;

  void *extraDriverData = NULL;
  //Common_Init(&extraDriverData);

  FMOD::Studio::System* system = NULL;
  ERRCHECK( FMOD::Studio::System::create(&system) );

  // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
  FMOD::System* coreSystem = NULL;
  ERRCHECK( system->getCoreSystem(&coreSystem) );
  ERRCHECK( coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0) );
  ERRCHECK( system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData) );
  
  FMOD::Studio::Bank* masterBank = NULL;
  ERRCHECK( system->loadBankFile(/*Common_MediaPath(*/"./media/Master.bank"/*)*/, FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );
  
  FMOD::Studio::Bank* stringsBank = NULL;
  ERRCHECK( system->loadBankFile(/*Common_MediaPath(*/"./media/Master.strings.bank"/*)*/, FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );
  
  FMOD::Studio::EventDescription* loopingAmbienceDescription = NULL;
  ERRCHECK( system->getEvent("event:/Ej1", &loopingAmbienceDescription) );
  
  FMOD::Studio::EventInstance* loopingAmbienceInstance = NULL;
  ERRCHECK( loopingAmbienceDescription->createInstance(&loopingAmbienceInstance) );
  
  // Get the 4 Second Surge event
  FMOD::Studio::EventDescription* musicDescription = NULL;
  ERRCHECK( system->getEvent("event:/Ej2", &musicDescription) );
  
  FMOD::Studio::EventInstance* musicInstance = NULL;
  ERRCHECK( musicDescription->createInstance(&musicInstance) );
  
  // Get the Single Explosion event
  FMOD::Studio::EventDescription* liftDescription = NULL;
  ERRCHECK( system->getEvent("event:/Ej3", &liftDescription) );
  // Start loading explosion sample data and keep it in memory
  //ERRCHECK( explosionDescription->loadSampleData() );
  // One-shot event
  FMOD::Studio::EventInstance* liftInstance = NULL;
  ERRCHECK( liftDescription->createInstance(&liftInstance) );
  do
  {
      Common_Update();
      
      if (Common_BtnPress(BTN_ACTION1))
    {
          ERRCHECK( liftInstance->start() );
          // Release will clean up the instance when it completes
          //ERRCHECK( eventInstance->release() );
      }
  
      if (Common_BtnPress(BTN_ACTION2))
      {
          ERRCHECK( loopingAmbienceInstance->start() );
      }

      if (Common_BtnPress(BTN_ACTION3))
      {
          ERRCHECK( loopingAmbienceInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
      }

      if (Common_BtnPress(BTN_ACTION4))
      {
          // Calling start on an instance will cause it to restart if it's already playing
          ERRCHECK( musicInstance->start() );
      }

      if (Common_BtnPress(BTN_ACTION5))
      {
          // Calling start on an instance will cause it to restart if it's already playing
          ERRCHECK( loopingAmbienceInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
          ERRCHECK( liftInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
          //ERRCHECK( musicInstance->stop(FMOD_STUDIO_STOP_IMMEDIATE) );
          
          if( nivel < 10) {
              nivel ++;
          }
          else {
              nivel = 0;
          }

          liftInstance->setParameterByName("nivel", nivel);
          std::cout << "Nivel: " << nivel << std::endl;
      }

      if (Common_BtnPress(BTN_ACTION6)) {
          if (hora < 24) {
              hora ++;
          }
          else {
              hora = 0;
          }

          loopingAmbienceInstance->setParameterByName("hora", hora);
          std::cout << "Hora: " << hora << std::endl;
      }

      if (Common_BtnPress(BTN_ACTION7)) {
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

      //Common_Draw("==================================================");
      //Common_Draw("Simple Event Example.");
      //Common_Draw("Copyright (c) Firelight Technologies 2012-2019.");
      //Common_Draw("==================================================");
      //Common_Draw("");
      //Common_Draw("Press %s to fire and forget the explosion", Common_BtnStr(BTN_ACTION1));
      //Common_Draw("Press %s to start the looping ambience", Common_BtnStr(BTN_ACTION2));
      //Common_Draw("Press %s to stop the looping ambience", Common_BtnStr(BTN_ACTION3));
      //Common_Draw("Press %s to start/restart exercice 3 sound", Common_BtnStr(BTN_ACTION4));
      //Common_Draw("Press %s to change 'nivel'", Common_BtnStr(BTN_ACTION5));
      //Common_Draw("Press %s to change 'hora'", Common_BtnStr(BTN_ACTION6));
      //Common_Draw("Press %s to change 'progreso'", Common_BtnStr(BTN_ACTION7));
      //Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));

      Common_Sleep(50);
  } while (!Common_BtnPress(BTN_QUIT));
  
  ERRCHECK( stringsBank->unload() );
  ERRCHECK( masterBank->unload() );

  ERRCHECK( system->release() );

  //Common_Close();

  return 0;


//    Game *game = Game::GetInstance();
//    
//    game->SetState(State::States::INGAME);
//    game->InitGame();
//
//
//    EventManager eventManager = EventManager::GetInstance();
//    eventManager.Suscribe(Listener {EventType::PRIORIDAD1,pruebaEvent1, "suscriptor1"});
//    eventManager.Suscribe(Listener {EventType::PRIORIDAD1,pruebaEvent1, "suscriptor2"});
//    eventManager.Suscribe(Listener {EventType::PRIORIDAD2,pruebaEvent2, "suscriptor3"});
//
//    Data d;
//
//    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
//    eventManager.AddEvent(Event {EventType::PRIORIDAD2,d});
//    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
//    
//    eventManager.Update();  
//    cout << "------------------------------\n";
//    eventManager.UnSuscribe(EventType::PRIORIDAD1,"suscriptor2");
//
//    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
//    eventManager.AddEvent(Event {EventType::PRIORIDAD2,d});
//    eventManager.AddEvent(Event {EventType::PRIORIDAD1,d});
//
//    
//    eventManager.Update();  
//
//    GameObject *go = new GameObject();
//
//    Component* cId   = new CId();
//    Component* cType = new CType(ModelType::Sphere);
//    Component* cTransformable = new CTransformable(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0);
//    Component* cTexture = new CTexture(std::string("wall.bmp"));
//    Component* cMesh   = new CMesh(std::string("media/ninja.b3d"));
//    go->AddComponent(cId);
//    go->AddComponent(cType);
//    go->AddComponent(cTransformable);
//    go->AddComponent(cTexture);
//    go->AddComponent(cMesh);
//
//	RenderFacadeManager renderFacadeManager = RenderFacadeManager::GetInstance();
//	renderFacadeManager.InitializeIrrlicht();
//
//    //InputFacadeManager* inputFacadeManager = new InputFacadeManager();
//    //inputFacadeManager->InitializeIrrlicht();
//    
//    //auto inputEngine  = inputFacadeManager->GetInputFacade();
//	auto renderEngine = renderFacadeManager.GetRenderFacade();
//	renderEngine->FacadeAddObject(go);
//    
//    renderEngine->FacadeAddCamera();
//
//    int lastFPS = -1;
//    uint32_t then = renderEngine->FacadeGetTime();
//
//    while(renderEngine->FacadeRun()){
//        const uint32_t now = renderEngine->FacadeGetTime();
//        
//        const float frameDeltaTime = (float)(now - then) / 1000.0;
//        then = now;
//        //inputEngine->CheckInputs();
//        renderEngine->FacadeCheckInput(frameDeltaTime);
//
//        renderEngine->FacadeDraw();
//
//
//        int fps = renderEngine->FacadeGetFPS();
//		if (lastFPS != fps)
//		{
//			std::string tmp("Movement Example - Irrlicht Engine [");
//			tmp += "] fps: ";
//			tmp += fps;
//
//			renderEngine->FacadeSetWindowCaption(tmp);
//			lastFPS = fps;
//		}
//
//    }
//
//    renderEngine->FacadeDeviceDrop();
//	
//	return 0;
}
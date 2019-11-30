#include "Entities/Hero.h"
#include "Entities/GameObject.h"
#include "Entities/Camera.h"
#include "Systems/Physics.h"
#include "EventManager/EventManager.h"
#include "Facade/Render/RenderFacadeManager.h"
#include "Facade/Input/InputFacadeManager.h"
#include "Facade/Physics/PhysicsFacadeManager.h"

#include "Game.h"

#include <iostream>
#include <list>
#include <string>
#include <cstdint>
#include <memory>

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "common.h"


int main()
{
  float nivel = 0;
  float hora = 0;
  float progreso = 0;

  void *extraDriverData = NULL;
  Common_Init(&extraDriverData);

  FMOD::Studio::System* system = NULL;
  ERRCHECK( FMOD::Studio::System::create(&system) );

  // The example Studio project is authored for 5.1 sound, so set up the system output mode to match
  FMOD::System* coreSystem = NULL;
  ERRCHECK( system->getCoreSystem(&coreSystem) );
  ERRCHECK( coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0) );
  ERRCHECK( system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData) );
  
  FMOD::Studio::Bank* masterBank = NULL;
  ERRCHECK( system->loadBankFile(Common_MediaPath("Master.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank) );
  
  FMOD::Studio::Bank* stringsBank = NULL;
  ERRCHECK( system->loadBankFile(Common_MediaPath("Master.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank) );
  
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

      Common_Draw("==================================================");
      Common_Draw("Simple Event Example.");
      Common_Draw("Copyright (c) Firelight Technologies 2012-2019.");
      Common_Draw("==================================================");
      Common_Draw("");
      Common_Draw("Press %s to fire and forget the explosion", Common_BtnStr(BTN_ACTION1));
      Common_Draw("Press %s to start the looping ambience", Common_BtnStr(BTN_ACTION2));
      Common_Draw("Press %s to stop the looping ambience", Common_BtnStr(BTN_ACTION3));
      Common_Draw("Press %s to start/restart exercice 3 sound", Common_BtnStr(BTN_ACTION4));
      Common_Draw("Press %s to change 'nivel'", Common_BtnStr(BTN_ACTION5));
      Common_Draw("Press %s to change 'hora'", Common_BtnStr(BTN_ACTION6));
      Common_Draw("Press %s to change 'progreso'", Common_BtnStr(BTN_ACTION7));
      Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));

      Common_Sleep(50);
  } while (!Common_BtnPress(BTN_QUIT));
  
  ERRCHECK( stringsBank->unload() );
  ERRCHECK( masterBank->unload() );

  ERRCHECK( system->release() );

  Common_Close();

  return 0;

//    EventManager* eventManager = EventManager::GetInstance();
//
//    GameObject *car = new GameObject(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0, "particle.bmp", "ninja.b3d",20,0.15,0.1,0.25);
//    GameObject *ground = new GameObject(10.0,10.0,30.0,    0.0,0.0,0.0,    100.0,1.0,100.0, "wall.jpg", "ninja.b3d",20,0.15,0.1,0.25);
//    Camera *cam = new Camera(10.0,20.0,30.0,    0.0,0.0,0.0,    1.0,1.0,1.0);
//
//	RenderFacadeManager* renderFacadeManager = RenderFacadeManager::GetInstance();
//	renderFacadeManager->InitializeIrrlicht();
//
//    InputFacadeManager* inputFacadeManager = InputFacadeManager::GetInstance();
//    inputFacadeManager->InitializeIrrlicht();
//
//    PhysicsFacadeManager* physicsFacadeManager = PhysicsFacadeManager::GetInstance();
//    physicsFacadeManager->InitializeIrrlicht();
//    
//    //Almacenamos los motores
//	auto renderEngine   = renderFacadeManager->GetRenderFacade();
//    auto inputEngine    = inputFacadeManager->GetInputFacade();
//    auto physicsEngine  = physicsFacadeManager->GetPhysicsFacade();
//
//	const uint16_t carID    = renderEngine->FacadeAddObject(car);
//    const uint16_t groundID = renderEngine->FacadeAddObject(ground);
//    
//    renderEngine->FacadeAddCamera(cam);
//
//    int lastFPS = -1;
//    uint32_t then = renderEngine->FacadeGetTime();
//
//    //GameLoop
//    while(renderEngine->FacadeRun()){
//        eventManager->Update();  
//        const uint32_t now = renderEngine->FacadeGetTime();
//        
//        const float frameDeltaTime = (float)(now - then) / 100.0;
//        then = now;
//        //inputEngine->CheckInputs(*car);
//        renderEngine->FacadeCheckInput(frameDeltaTime,*car,*cam);
//        renderEngine->UpdateTransformable(car);
//        renderEngine->UpdateCamera(cam);
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
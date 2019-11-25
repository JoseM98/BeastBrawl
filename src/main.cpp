#include "entities/hero.h"
#include "components/position.h"
#include "components/speed.h"
#include "systems/physics.h"
#include "EventManager/EventManager.h"

#include "Game.h"
#include <iostream>
#include <list>
#include <string>
#include <math.h>





#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;

const  float PI = 3.14159;

void pruebaEvent1(Data d){
    
    std::cout << "Soy pruebaEvent1"<< "\n";
}
void pruebaEvent2(Data d){
    std::cout << "Soy pruebaEvent2"<< "\n";
}



class MyEventReceiver : public IEventReceiver
{
public:
  // This is the one method that we have to implement
  virtual bool OnEvent(const SEvent& event)
  {
    // Remember whether each key is down or up
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
      KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

    return false;
  }
  // This is used to check whether a key is being held down
  virtual bool IsKeyDown(EKEY_CODE keyCode) const
  {
    return KeyIsDown[keyCode];
  }
  MyEventReceiver()
  {
    for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
      KeyIsDown[i] = false;
  }
private:
  // We use this array to store the current state of each key
  bool KeyIsDown[KEY_KEY_CODES_COUNT];
};




int main()
{
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



// --------------------------- IRLICH ---------------------------------------
// ask user for driver
  video::E_DRIVER_TYPE driverType=driverChoiceConsole();
  if (driverType==video::EDT_COUNT)
    return 1;


  // create device
  MyEventReceiver receiver;
  IrrlichtDevice* device = createDevice(driverType,
      core::dimension2d<u32>(1280, 720), 16, false, false, false, &receiver);

  if (device == 0)
    return 1; // could not create selected driver.

  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();

  /*
  Create the node which will be moved with the WSAD keys. We create a
  sphere node, which is a built-in geometry primitive. We place the node
  at (0,0,30) and assign a texture to it to let it look a little bit more
  interesting. Because we have no dynamic lights in this scene we disable
  lighting for each model (otherwise the models would be black).
  */
  scene::ISceneNode * node = smgr->addCubeSceneNode();
  if (node)
  {
    node->setPosition(core::vector3df(0,0,30));
    node->setMaterialTexture(0, driver->getTexture("media/particle.bmp"));
    node->setMaterialFlag(video::EMF_LIGHTING, false);
  }

  scene::ISceneNode * node2 = smgr->addCubeSceneNode();
  if (node2)
  {
    node2->setPosition(core::vector3df(0,-10,30));
    node2->setMaterialTexture(0, driver->getTexture("media/wall.jpg"));
    node2->setMaterialFlag(video::EMF_LIGHTING, false);

    node2->setScale(core::vector3df(100,1,100));
  }
  /*
  To be able to look at and move around in this scene, we create a first
  person shooter style camera and make the mouse cursor invisible.
  */
    scene::ICameraSceneNode* camera = smgr->addCameraSceneNode();
    if (camera)
    {
        camera->setPosition(core::vector3df(0, 20, 0));
        camera->setTarget(core::vector3df(0,0,30));
        //camera->setRotation(core::vector3df(0, 90, 0));  // La rotación se define mediante el target
        //camera->bindTargetAndRotation(true);
        //camera->setFarValue(20.0f);
    }
  
  device->getCursorControl()->setVisible(false);

  /*
  Add a colorful irrlicht logo
  */
// aqui podria ir un marcador o algo 
  device->getGUIEnvironment()->addImage(
    driver->getTexture("media/irrlichtlogoalpha2.tga"),
    core::position2d<s32>(10,20));

  gui::IGUIStaticText* diagnostics = device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(10, 10, 400, 20));
  diagnostics->setOverrideColor(video::SColor(255, 255, 255, 0));

  /*
  We have done everything, so lets draw it. We also write the current
  frames per second and the name of the driver to the caption of the
  window.
  */
  int lastFPS = -1;

  // In order to do framerate independent movement, we have to know
  // how long it was since the last frame
  u32 then = device->getTimer()->getTime();

  // This is the movemen speed in units per second.
    //const f32 MOVEMENT_SPEED = 20.f;


    float actualRotation = 0.f;  // rotacion ruedas del coche -> no se ve visiblemente en la escena
    float angleRotation = 0.f;
    const float maxSpeed = 20.f;
    const float carFriction = 0.1;
    const float carAcceleration = 0.15;
    const float carSlowDown = 0.25;
    float carSpeed = 0;
    float lateralCarCamera = 0.f;
    // To-Do: Poner un tiempo para que al pulsar rapido gire poco y al mantener gire mas
    // To-Do: Que al girar se vea un lateral un poco del coche
    // To-Do: Ponerle marchas automaticas de forma que al principio acelere muy rapido y luego le cueste mas
    // To-Do: El target se queda por delante del jugador


while(device->run())
  {
    angleRotation = 0.f;

    // Work out a frame delta time.
    const u32 now = device->getTimer()->getTime();
    const f32 frameDeltaTime = (f32)(now - then) / 100.f; // Time in seconds
    then = now;

    /* Check if keys W, S, A or D are being held down, and move the
    sphere node around respectively. */
    core::vector3df nodePosition = node->getPosition();
    core::vector3df nodeRotation = node->getRotation();
    core::vector3df cameraPosition = camera->getPosition();

    if(receiver.IsKeyDown(irr::KEY_KEY_W)){
      carSpeed += carAcceleration;
      if(carSpeed>maxSpeed)
        carSpeed = maxSpeed;
    }else if(receiver.IsKeyDown(irr::KEY_KEY_S)){
      carSpeed -= carSlowDown;
      if(carSpeed < 0)
        carSpeed = 0;
      //nodePosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
      //cameraPosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
    }else{
      if(carSpeed > 0) {
        carSpeed -= carFriction;
        if(carSpeed < 0)
          carSpeed = 0;
      }
    }

    std::cout << "Car Speed: " << carSpeed << std::endl;
    if(receiver.IsKeyDown(irr::KEY_KEY_A)){
        if(carSpeed>3){
            if(actualRotation>-10){
              actualRotation -= 0.5;
            }
            if(lateralCarCamera>-15){
              lateralCarCamera -= 0.5;
            }
        }
      //nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
      //cameraPosition.X -= MOVEMENT_SPEED * frameDeltaTime;
    }else if(receiver.IsKeyDown(irr::KEY_KEY_D)){
        if(carSpeed>3){
            if(actualRotation<10){
              actualRotation += 0.5;
            }
            if(lateralCarCamera<15){
              lateralCarCamera += 0.5;
            }
        }
      //nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
      //cameraPosition.X += MOVEMENT_SPEED * frameDeltaTime;
    }else{
        if(actualRotation>=0.7)
            actualRotation -= 0.7;
        else if(actualRotation<=-0.7)
            actualRotation += 0.7;
        else
            actualRotation = 0;
        
        if(lateralCarCamera>=0.7)
            lateralCarCamera -= 0.7;
        else if(lateralCarCamera<=-0.7)
            lateralCarCamera += 0.7;
        else
            lateralCarCamera = 0;
    }

    // Calcular la posicion
    if(actualRotation!=0){
        angleRotation=(nodeRotation.Y*PI)/180.0;
        nodePosition.X += sin(angleRotation)*carSpeed * frameDeltaTime;
        nodePosition.Z += cos(angleRotation)*carSpeed * frameDeltaTime;
        nodeRotation.Y += actualRotation * 0.20;
      }else{
        angleRotation=(nodeRotation.Y*PI)/180.0;
        nodePosition.Z += cos(angleRotation)*carSpeed * frameDeltaTime;
        nodePosition.X += sin(angleRotation)*carSpeed * frameDeltaTime;
      }

    if(receiver.IsKeyDown(irr::KEY_ESCAPE))
      break;
    

    node->setRotation(nodeRotation);
    node->setPosition(nodePosition);
    //camera->setTarget(nodePosition);

    // Camara Target
    core::vector3df targetPosition  = nodePosition;
    targetPosition.Y += 17;
    camera->setTarget(targetPosition);

    // Camara Position
    cameraPosition.Y = nodePosition.Y + 20;
    cameraPosition.X = nodePosition.X-40.0*sin(((nodeRotation.Y-lateralCarCamera)*PI)/180.0);
    cameraPosition.Z = nodePosition.Z-40.0*cos(((nodeRotation.Y-lateralCarCamera)*PI)/180.0);
    camera->setPosition(cameraPosition);
    //std::cout << camera->getTargetAndRotationBinding() << " -> valor camara target" << std::endl;
    // camera->bindTargetAndRotation(false);
    //camera->updateAbsolutePosition();

    driver->beginScene(true, true, video::SColor(255,113,113,133));

    smgr->drawAll(); // draw the 3d scene
    device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

    driver->endScene();

    int fps = driver->getFPS();

    if (lastFPS != fps)
    {
      core::stringw tmp(L"Movement Example - Irrlicht Engine [");
      tmp += driver->getName();
      tmp += L"] fps: ";
      tmp += fps;

      device->setWindowCaption(tmp.c_str());
      lastFPS = fps;
    }
  }

  /*
  In the end, delete the Irrlicht device.
  */
  device->drop();
  
  return 0;
}

/*
That's it. Compile and play around with the program.
**/



    //return 0;
    /*
    // creamos a nuestro hero
    Hero *hero = new Hero();
    cout << "Nuestro hero tiene position? " << hero->HasComponent(CompType::Position) << endl;


    // creamos nuestro componente position
    CPosition *pos = new CPosition(1.2, 4.3, -3.1);
    hero->AddComponent(pos);

    cout << "Nuestro hero tiene position? " << hero->HasComponent(CompType::Position) << endl;

    // ahora obtenemos la posición a partir de la entidad que hemos creado
    //CPosition *posObtenida = (CPosition *) h->getComponents().at(CompType::Position); // alternativa 2 a obtener el componente
    CPosition *posObtenida = (CPosition *) hero->getComponents()[CompType::Position];
    cout << "La posición de nuestro hero es " << *posObtenida << endl;

    // También se podría hacer de una forma más compacta directamente usando el hero
    cout << "La posición de nuestro hero es " << *(CPosition *)(hero->getComponents()[CompType::Position]) << endl;

    
    // ahora creamos un componente velocidad
    CSpeed *speed = new CSpeed(2.7, 7.1, -1.1);

    // y se la agregamos a nuestro hero igual que antes
    hero->AddComponent(speed);

    // y la mostramos por pantalla
    cout << "La velocidad de nuestro hero es " << *(CPosition *)(hero->getComponents()[CompType::Speed]) << endl;


    // Ahora movemos a nuestro hero usando un sistema physics
    // requiere que le mandemos el componente speed y el componente position, y él hace el cálculo.
    // como veis, no necesita de la entidad, por tanto es totalmente reutilizable para cualquier 
    // entidad que tenga speed y position
    Physics::move(
        (CSpeed *)hero->getComponents()[CompType::Speed], 
        (CPosition *)hero->getComponents()[CompType::Position]
    );

    // y mostramos su nueva posición por pantalla
    cout << "La posición de nuestro hero es " << *(CPosition *)(hero->getComponents()[CompType::Position]) << endl;


    delete speed;
    delete posObtenida;
    delete hero;*/
//}
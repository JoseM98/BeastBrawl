#pragma once

#include <irrlicht/irrlicht.h>
#include <EventManager/Event.h>
#include <EventManager/EventManager.h>
#include <Facade/Input/InputFacadeIrrlicht.h>
#include <Facade/Input/InputFacadeManager.h>
#include "RenderFacade.h"

//#include "../../State/State.h"

#include <codecvt>
#include <iostream>
#include <locale>
#include <memory>
#include <string>
#include <unordered_map>



using namespace std;
using namespace irr;

class RenderFacadeIrrlicht : public RenderFacade {
   public:
    RenderFacadeIrrlicht();
    ~RenderFacadeIrrlicht() override;
    const uint16_t FacadeAddObject(Entity*) override;
    const uint16_t FacadeAddObjectCar(Entity*) override;
    const uint16_t FacadeAddObjectTotem(Entity* entity) override;
    const void FacadeAddObjects(vector<Entity*>) override;
    void FacadeAddCamera(Entity*) override;
    void UpdateTransformable(Entity*) override;
    void UpdateCamera(Entity*, ManCar* manCars) override;
    bool FacadeRun() override;
    uint32_t FacadeGetTime() const override;
    void FacadeCheckInput() override;
    int FacadeGetFPS() const override;
    void FacadeSetWindowCaption(std::string, int) const override;
    void FacadeBeginScene() const override;
    void FacadeDrawAll() const override;
    void FacadeEndScene() const override;
    void FacadeDeviceDrop() override;
    void FacadeDraw() const override;
    void DeleteEntity(Entity*) override;
    void FacadeDrawMenu() override;
    void FacadeDrawPause() override;
    void FacadeDrawEndRace() override;
    void FacadeInitMenu() override;
    void FacadeInitPause() override;
    void FacadeInitEndRace() override;
    void FacadeInitHUD() override;
    void FacadeCheckInputMenu() override;
    void FacadeCheckInputPause() override;
    void FacadeCheckInputEndRace() override;
    void FacadeUpdatePowerUpHUD(DataMap* d) override;
    void FacadeDrawHUD(Entity* car, ManCar* carsAI) override;
    void FacadeSuscribeEvents() override;
    void FacadeAddPlates(Manager* manNamePlates) override;
    void FacadeUpdatePlates(Manager* manNamePlates) override;

    //DEBUG
    void Draw3DLine(vec3& pos1, vec3& pos2, uint16_t r, uint16_t g, uint16_t b) const override;
    void Draw3DLine(vec3& pos1, vec3& pos2) const override;
    void FacadeDrawGraphEdges(ManWayPoint* manWayPoints) const override;
    void FacadeDrawBoundingBox(Entity* entity, bool colliding) const override;
    void FacadeDrawBoundingPlane(Entity* entity) const override;
    void FacadeDrawAIDebug(ManCar* manCars, ManNavMesh* manNavMesh, ManWayPoint* manWayPoint) const override;
    void FacadeDrawAIDebugPath(Entity* carAI, ManWayPoint* manWayPoint) const override;



    scene::ISceneManager* GetSceneManager() { return smgr; };
    scene::ICameraSceneNode* GetCamera1() { return camera1; };
    static bool showDebug;
    static bool showAIDebug;

   private:
    IrrlichtDevice* device;
    video::IVideoDriver* driver;
    scene::ISceneManager* smgr;
    scene::ICameraSceneNode* camera1;
    MyEventReceiver receiver;
    video::ITexture* menuBG;
    video::ITexture* pauseBG;
    video::ITexture* endRaceBG;
    video::ITexture* whiteBG;
    video::ITexture* powerUps[7];
    gui::IGUIFont* font;
    int currentPowerUp = 0;

    //unordered_map<uint16_t,scene::ISceneNode*> sceneObjects; // CId - ISceneNode*
};

#pragma once

#include "RenderFacade.h"
#include "../../../lib/irrlicht/irrlicht.h"
#include "../../EventManager/EventManager.h"
#include "../../EventManager/Event.h"
#include "../Input/InputFacadeIrrlicht.h"
#include "../Input/InputFacadeManager.h"

#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;
using namespace irr;


class RenderFacadeIrrlicht : public RenderFacade {
    public:
        RenderFacadeIrrlicht();
        ~RenderFacadeIrrlicht() override;
        const uint16_t FacadeAddObject(Entity*) override;
        const uint16_t FacadeAddObjectCar(Entity*) override;
		const void FacadeAddObjects(vector<Entity*>) override;
        void UpdateTransformable(Entity*) override;
        void UpdateCamera(Entity*) override;
		void FacadeAddCamera(Entity*) override;
		bool FacadeRun() override;
		uint32_t FacadeGetTime() override;
		void FacadeCheckInput() override;
		int FacadeGetFPS() override;
		void FacadeSetWindowCaption(std::string) override;
		void FacadeBeginScene() override;
		void FacadeDrawAll() override;
		void FacadeEndScene() override;
		void FacadeDeviceDrop() override;
		void FacadeDraw() override;

		//DEBUG
        void FacadeDrawGraphEdges(ManWayPoint* manWayPoints) override;
		void FacadeDrawBoundingBox(Entity* entity, bool colliding) override;


		scene::ISceneManager* GetSceneManager() {return smgr;};
		scene::ICameraSceneNode* GetCamera1() {return camera1;};


    private:
        IrrlichtDevice* device;
        video::IVideoDriver* driver;
        scene::ISceneManager* smgr;
		scene::ICameraSceneNode* camera1;
		MyEventReceiver receiver;
		bool showDebug = false;

        //unordered_map<uint16_t,scene::ISceneNode*> sceneObjects; // CId - ISceneNode*
};


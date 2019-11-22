#ifndef __RENDERFACADEIRRLICHT_H__
#define __RENDERFACADEIRRLICHT_H__

#include <iostream>
#include "RenderFacade.h"
#include <irrlicht.h>
#include <unordered_map>

using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == EET_KEY_INPUT_EVENT)
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

class RenderFacadeIrrlicht : public RenderFacade {
    public:
        RenderFacadeIrrlicht();
        ~RenderFacadeIrrlicht();
        uint16_t FacadeAddObject(GameObject*) override;
		void FacadeAddCamera() override;
		bool FacadeRun() override;
		uint32_t FacadeGetTime() override;
		void FacadeCheckInput(float) override;
		int FacadeGetFPS() override;
		void FacadeSetWindowCaption(std::string) override;
		void FacadeBeginScene() override;
		void FacadeDrawAll() override;
		void FacadeEndScene() override;
		void FacadeDeviceDrop() override;
		void FacadeDraw() override;

    private:
        IrrlichtDevice* device;
        video::IVideoDriver* driver;
        scene::ISceneManager* smgr;
        MyEventReceiver receiver;

        unordered_map<uint16_t,scene::ISceneNode*> sceneObjects; // CId - ISceneNode*
};


#endif 
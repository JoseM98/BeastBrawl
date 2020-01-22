#pragma once

#include <iostream>
#include <memory>
#include "InputFacade.h"
//#include "../Render/RenderFacadeManager.h"
#include "../../../include/irrlicht/irrlicht.h"

using namespace std;
using namespace irr;


class MyEventReceiver : public IEventReceiver{

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

class InputFacadeIrrlicht : public InputFacade{
    public:
        InputFacadeIrrlicht();
        ~InputFacadeIrrlicht();
        void CheckInput()        override;
        void CheckInputMenu()    override;
        void CheckInputPause()   override;
        void CheckInputEndRace() override;

    private:
        MyEventReceiver receiver;
		IrrlichtDevice *device;

};

#pragma once

#include <iostream>
#include <memory>
#include <GameValues.h>

#include "InputFacade.h"
#include "../Render/RenderFacadeManager.h"
#include "../Sound/SoundFacadeManager.h"

#include "../../../CLEngine/src/CLEngine.h"

using namespace std;
using namespace CLE;

class InputFacadeClover : public InputFacade{
    public:
        InputFacadeClover();
        ~InputFacadeClover() override = default;

        void CheckInputIntro() override;
        void CheckInputMenu(int&, int)    override;

        void CheckInputSelectCharacter(int&, int) override;
        void CheckInputGameOptions(std::vector<int>&, int[], int&) override;

        bool ShowTable(bool) override;
        bool CheckInputSingle()  override;
        void CheckInputLobbyMulti() override;
        vector<Constants::InputTypes> CheckInputMulti()override;

        void CheckInputPause(int&, int)   override;
        void CheckInputEndRace(int&, int, bool) override;
        void CheckInputController() override;
        void CheckInputCredits() override;
        void CheckInputSettings(std::vector<int> &inputs, int maxInputs[], int& option) override;
        bool CheckInputAnimationsStartEnd() override;

        void SetWindowContext(CLEngine* d) {device = d;}

    private:

        CLEngine* device;

        enum InputXBox{BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y, BUTTON_LB, BUTTON_RB, BUTTON_BACK, BUTTON_START, BUTTON_XBOX, BUTTON_STICK_UP, BUTTON_STICK_DOWN, BUTTON_STICK_L, BUTTON_STICK_R, END};
        std::unordered_map<InputXBox, bool> inputsPressed;
        core::array<SJoystickInfo> joystickInfo;
        bool multiplayer { false };

        bool IsInputPressed(InputXBox input);
        void SetValueInput(InputXBox input, bool valuePressed);
        bool IsKeyOrGamepadPress(int key, int button1, bool axes, float axe, int button2, bool doubleGamepad);
        bool HasDelayPassed();

        bool WeHaveToGoToMenu {false}; // esto sirve para que se oiga el sonidito de pasar desde pausa ingame hasta menú, es una chapuza pero no quiero perder más tiempo en esa chorrada
        uint64_t timerGoToMenu {0};

        void ChangeGameOptions(int, int);
        void ChangeSettings(int, int);
};
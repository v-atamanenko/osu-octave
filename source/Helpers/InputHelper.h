#include <vector>
#include <map>
#include "SDL.h"
#include "types.h"
#include "DataStorage/Settings.h"
#include <algorithm>

#ifndef __INPUTHELPER_H__
#define __INPUTHELPER_H__

class InputHelper
{
	public:
		static bool KeyDown(Control c);
		static bool KeyHeld(Control c);
		static bool KeyUp(Control c);
        static void SimulateKeyDown(Control c);
        static void SimulateKeyUp();
		static touchPosition& TouchRead();
        static int PollSDL();
        static void ClearInput();
        static void InitInput();
        static bool BlockKeydown;
        static bool vitaUseBackTouch;

	protected:
        static std::map<Control, std::vector<RawKey>> mControls;
        static std::vector<SDL_Event> sdlEvents;
        static std::vector<SDL_Event> simulatedKeyDowns;
        static std::vector<int> heldControllerKeys;
        static std::vector<int> heldKeyboardKeys;
        static std::vector<int> heldMouseButtons;
		static touchPosition mTouch;
};

#endif


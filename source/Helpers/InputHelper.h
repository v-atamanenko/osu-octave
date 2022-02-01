#include <vector>
#include "SDL.h"

#ifndef __INPUTHELPER_H__
#define __INPUTHELPER_H__

#define IH_KEY_MOUSE 0
#define IH_KEY_CONTROLLER 1
#define IH_KEY_KEYBOARD 2

struct touchPosition {
    uint32_t px;
    uint32_t py;
};

class InputHelper
{
	public:
		static bool KeyDown(int key, int type);
		static bool KeyHeld(int key, int type);
		static bool KeyUp(int key, int type);
		static touchPosition& TouchRead();
        static int PollSDL();
	
	protected:
        static std::vector<SDL_Event> sdlEvents;
        static std::vector<int> heldControllerKeys;
        static std::vector<int> heldKeyboardKeys;
        static std::vector<int> heldMouseButtons;
		static touchPosition mTouch;
};

#endif

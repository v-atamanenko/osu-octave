#pragma once

#include <vector>
#include <map>
#include "SDL.h"
#include "types.h"
#include "DataStorage/Settings.h"
#include <algorithm>

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
        static void InitInput();
        static bool vitaUseBackTouch;

	protected:
        static std::map<Control, std::vector<RawKey>> mControls;
        static std::vector<SDL_Event> simulatedKeyDowns;
        static std::vector<RawKey> blockedKeys;
        static std::vector<RawKey> uppedKeys;
        static std::vector<RawKey> downedKeys;
		static touchPosition mTouch;

        static inline bool checkKeyState(std::vector<RawKey>& v, const RawKey &k);
        static inline void releaseKey(const RawKey& k);
        static inline void holdKey(const RawKey& k);
};

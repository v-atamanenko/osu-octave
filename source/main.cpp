#include "System/SDLInitializer.h"

#include "System/GameBase.h"
#include "System/GameClock.h"
#include "System/TextManager.h"
#include "System/Settings.h"

#ifdef VITA
unsigned int _newlib_heap_size_user = 200 * 1024 * 1024;
#endif

json Settings::settings;

int main() {
    SDLInitializer::initSDL();
    TextManager::Init();
    Settings::load();

	GameBase osu;

    while(true) {
        if (osu.Update() == -1) {
            SDL_Quit();
            break;
        }

        GameClock::Clock().Update();
	}

    Settings::save();
	return 0;
}

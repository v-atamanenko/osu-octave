#include "System/SDLInitializer.h"

#include "System/GameBase.h"
#include "System/GameClock.h"
#include "System/TextManager.h"
#include "DataStorage/Settings.h"
#include "DataStorage/Scores.h"
#include "DataStorage/Betmaps.h"
#include "DataStorage/Skins.h"

#ifdef VITA
unsigned int _newlib_heap_size_user = 200 * 1024 * 1024;
#include <psp2/power.h>
#endif

int main() {
#ifdef __vita__
    scePowerSetArmClockFrequency(444);
#endif

    Settings::load();
    Skins::init_skin();
    SDLInitializer::initSDL();
    TextManager::Init();

	GameBase osu;
    while(osu.Update() != -1) {
        GameClock::Clock().Update();
	}

    SDL_Quit();
    Settings::save();
	return 0;
}

#include "System/SDLInitializer.h"

#include "System/GameBase.h"
#include "System/GameClock.h"
#include "System/TextManager.h"
#include "DataStorage/Settings.h"
#include "DataStorage/Scores.h"
#include "DataStorage/Betmaps.h"

#ifdef VITA
unsigned int _newlib_heap_size_user = 200 * 1024 * 1024;
#endif

json Settings::settings;
json Scores::scores;
json Beatmaps::beatmaps;
json Beatmaps::state;

int main() {
    Settings::load();
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

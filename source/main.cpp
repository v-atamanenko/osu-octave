#include "System/SDLInitializer.h"

#include "System/GameBase.h"
#include "System/GameClock.h"
#include "System/TextManager.h"

#ifdef VITA
unsigned int _newlib_heap_size_user = 200 * 1024 * 1024;
#endif

int main()
{
    SDLInitializer::initSDL();
    TextManager::Init();
	
	GameBase osu;

    while(true)
	{
        if (osu.Update() == -1) {
            SDL_Quit();
            break;
        }

        GameClock::Clock().Update();
	}

	return 0;
}

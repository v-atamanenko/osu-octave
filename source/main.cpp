#include "System/SDLInitializer.h"

#include "System/GameBase.h"
#include "System/GameClock.h"
#include "System/TextManager.h"

#define TICKS_FOR_NEXT_FRAME (1000 / 60)

#ifdef VITA
unsigned int _newlib_heap_size_user = 140 * 1024 * 1024;
#endif

int main()
{
    SDLInitializer::initSDL();
    TextManager::Init();
	
	GameBase osu;

    uint32_t lastTime = 0;

    while(true)
	{
        while ((lastTime - SDL_GetTicks()) < TICKS_FOR_NEXT_FRAME) {
            SDL_Delay(1);
        }

		if (osu.Update() == -1) {
            SDL_Quit();
            break;
        }

        //TextManager::Top().SetFont(FONT_CONSOLE);
		//TextManager::Top().Print("TIME: %i",GameClock::Clock().Time());

        GameClock::Clock().Update();
        lastTime = SDL_GetTicks();
	}

	return 0;
}

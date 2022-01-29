#include "sys_sdl2.h"

#include "System/GameBase.h"
#include "System/GameClock.h"
#include "System/TextManager.h"

#define TICKS_FOR_NEXT_FRAME (1000 / 60)

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
            break;
        }

        //TextManager::Top().SetFont(FONT_CONSOLE);
		//TextManager::Top().Print("TIME: %i",GameClock::Clock().Time());

        GameClock::Clock().Update();
        lastTime = SDL_GetTicks();
	}

	return 0;
}

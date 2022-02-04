#include "Mode.h"
#include "Player.h"
#include "SongSelect.h"
#include "Welcome.h"
#include "System/GameClock.h"
#include "System/TextManager.h"

void ChangeMode(ModeType mode)
{
	/*/dim screen
	if (Mode::sCurrentMode != NULL)
	{
		for (int b = 0; b >= -16; --b)
		{
			setBrightness(1, b);
			swiWaitForVBlank();
		}
	}
	else
	{
		setBrightness(1, -16);
	}*/
	
	TextManager::Bottom().Clear();
	TextManager::Top().Clear();
	
	//must reset clock before calling constructors
	//otherwise bad things happen with sprites
	GameClock::Clock().Reset();
	

    delete Mode::sCurrentMode; // remove any current mode if present
	
	switch (mode)
	{
		case MODE_PLAYER:
			Mode::sCurrentMode = new Player();
			break;
		case MODE_SONGSELECT:
			Mode::sCurrentMode = new SongSelect();
			break;
        case MODE_WELCOME:
            Mode::sCurrentMode = new Welcome();
            break;
	}
	
	//swiWaitForVBlank();
	Mode::CurrentMode().Update();
	//glFlush(1);
	
	/*/return screen to full brightness
	for (s32 b = -16; b <= 0; ++b)
	{
		setBrightness(1, b);
		swiWaitForVBlank();
	}*/
}

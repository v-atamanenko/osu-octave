#include "GameBase.h"
#include "TextManager.h"

GameBase::GameBase()
{
	BeatmapManager::BuildCollection();
	//ChangeModeOnFrameEnd(MODE_PLAYER);
    ChangeModeOnFrameEnd(MODE_WELCOME);
    ChangeModeOnDemand();
}

int GameBase::Update()
{
    if (InputHelper::PollSDL() == -1) return -1;
	Mode::CurrentMode().HandleInput();

    GraphicsManager::Graphics().Clear();
    TextManager::Top().Clear();

    Mode::CurrentMode().Update();
    TextManager::Top().drawTex();

    GraphicsManager::Graphics().Present();
    ChangeModeOnDemand();

    return 0;
}


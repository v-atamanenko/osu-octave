#include "GameBase.h"
#include "TextManager.h"

GameBase::GameBase()
{
	BeatmapManager::BuildCollection();
	//ChangeMode(MODE_PLAYER);
	ChangeMode(MODE_WELCOME);
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

    return 0;
}


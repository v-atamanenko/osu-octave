#include "GameBase.h"
#include "TextManager.h"

GameBase::GameBase()
{
	BeatmapManager::BuildCollection();
	//ChangeMode(MODE_PLAYER);
	ChangeMode(MODE_SONGSELECT);
}

int GameBase::Update()
{
    if (InputHelper::PollSDL() == -1) return -1;
	Mode::CurrentMode().HandleInput();

    GraphicsManager::Graphics().clear();
    TextManager::Top().Clear();

    GraphicsManager::Graphics().bgDraw();
	Mode::CurrentMode().Update();
    TextManager::Top().drawTex();

    GraphicsManager::Graphics().present();

    return 0;
}


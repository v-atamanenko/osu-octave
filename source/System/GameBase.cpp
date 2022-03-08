#include "GameBase.h"
#include "TextManager.h"
#include "../Helpers/PreviewBuffer.h"

GameBase::GameBase() {
	ChangeModeOnFrameEnd(MODE_WELCOME);
    ChangeModeOnDemand();
}

int GameBase::Update() {
    if (InputHelper::PollSDL() == -1) return -1;
	Mode::CurrentMode().HandleInput();

    GraphicsManager::Clear();

    Mode::CurrentMode().Update();

    GraphicsManager::Present();
    ChangeModeOnDemand();

    return 0;
}

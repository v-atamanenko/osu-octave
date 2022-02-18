#include "GameBase.h"
#include "TextManager.h"
#include "../Helpers/PreviewBuffer.h"

GameBase::GameBase() {
	BeatmapManager::BuildCollection();
    PreviewBuffer::GetInstance().Init();

    ChangeModeOnFrameEnd(MODE_WELCOME);
    ChangeModeOnDemand();
}

int GameBase::Update() {
    if (InputHelper::PollSDL() == -1) return -1;
	Mode::CurrentMode().HandleInput();

    GraphicsManager::Graphics().Clear();

    Mode::CurrentMode().Update();

    GraphicsManager::Graphics().Present();
    ChangeModeOnDemand();

    return 0;
}

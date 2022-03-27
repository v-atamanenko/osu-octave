#include "GameBase.h"
#include "TextManager.h"
#include "../Helpers/PreviewBuffer.h"

GameBase::GameBase() {
	ChangeModeOnFrameEnd(MODE_WELCOME);
    ChangeModeOnDemand();
}

int GameBase::Update() {
    int wantExit = InputHelper::PollSDL();

    if (wantExit == -1) {
        AudioManager::Engine().MusicStop();
        AudioManager::Engine().PlayUISound(UISOUND_SEEYA);
        SDL_Delay(1300);
        return -1;
    }

	Mode::CurrentMode().HandleInput();

    GraphicsManager::Clear();

    Mode::CurrentMode().Update();

    GraphicsManager::Present();
    ChangeModeOnDemand();

    return 0;
}

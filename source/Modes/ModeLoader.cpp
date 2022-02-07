#include "Mode.h"
#include "Player.h"
#include "SongSelect.h"
#include "Welcome.h"
#include "System/GameClock.h"
#include "System/TextManager.h"

void ChangeModeOnFrameEnd(ModeType mode) {
    Mode::wantChangeMode = true;
    Mode::wantChangeModeTo = mode;
}

void ChangeModeOnDemand() {
    if (!Mode::wantChangeMode) {
        return;
    }

    ModeType mode = Mode::wantChangeModeTo;

    GameClock::Clock().Reset();
    delete Mode::sCurrentMode; // remove any current mode if Present
	
	switch (mode) {
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

    Mode::wantChangeMode = false;
}

Mode::~Mode() {
    GraphicsManager::Graphics().UnloadTextures();
}

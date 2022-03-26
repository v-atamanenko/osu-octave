#include "Mode.h"
#include "Player.h"
#include "SongSelect.h"
#include "Welcome.h"
#include "ModeSettings.h"
#include "ModeAbout.h"
#include "System/GameClock.h"
#include "System/TextManager.h"

Mode* Mode::sCurrentMode = nullptr;
bool Mode::wantChangeMode = false;
ModeType Mode::wantChangeModeTo;

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
        case MODE_SETTINGS:
            Mode::sCurrentMode = new ModeSettings();
            break;
        case MODE_ABOUT:
            Mode::sCurrentMode = new ModeAbout();
            break;
    }

    Mode::wantChangeMode = false;
}

Mode::~Mode() {
    GraphicsManager::Graphics().UnloadTextures();
}

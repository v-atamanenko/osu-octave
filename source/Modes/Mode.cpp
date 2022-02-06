#include "Mode.h"
#include "defines.h"

Mode* Mode::sCurrentMode = nullptr;
bool Mode::wantChangeMode = false;
ModeType Mode::wantChangeModeTo;

void Mode::ChangeToOsuDir()
{
    chdir(DATA_DIR);
}

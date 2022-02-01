#include "Mode.h"
#include "defines.h"

Mode* Mode::sCurrentMode = nullptr;

void Mode::ChangeToOsuDir()
{
    chdir(DATA_DIR);
}

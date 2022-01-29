#include "Mode.h"

Mode* Mode::sCurrentMode = nullptr;

void Mode::ChangeToOsuDir()
{
    //chdir("ux0:data/osu");
    chdir("/Users/v/Repos/osu-ds/cmake-build-debug");
}

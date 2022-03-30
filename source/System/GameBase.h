#pragma once

#include <cstdio>

#include "GameClock.h"
#include "Modes/Mode.h"
#include "Beatmaps/BeatmapManager.h"
#include "TextManager.h"
#include "../Helpers/PreviewBuffer.h"

class GameBase
{
	public:
		GameBase();
		int Update();
};

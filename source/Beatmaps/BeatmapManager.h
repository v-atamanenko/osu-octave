#include <cstdio>
#include <string>
#include <dirent.h>
#include <cstring>
#include <vector>

#include "Beatmap.h"
#include "BeatmapElements.h"
#include "DifficultyManager.h"
#include "Modes/Mode.h"

#ifndef __BEATMAPMANAGER_H__
#define __BEATMAPMANAGER_H__

typedef std::vector<Beatmap*>::iterator beatmapIterator;

class BeatmapManager
{
	public:
		static Beatmap& Current() { return *mBeatmapCurrent; }
		
		static void BuildCollection();
		static void Load(uint32_t index);
		static uint32_t MapCount();
		static uint32_t SongCount();
		
		static std::vector<Beatmap*>& Beatmaps() { return mBeatmaps; }
	
	protected:
		static Beatmap* mBeatmapCurrent;
		
		static std::vector<Beatmap*> mBeatmaps;
};

#endif


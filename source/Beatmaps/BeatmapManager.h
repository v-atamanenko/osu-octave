#include <cstdio>
#include <string>
#include <dirent.h>
#include <cstring>
#include <vector>

#include "Beatmap.h"
#include "BeatmapElements.h"
#include "DifficultyManager.h"
#include "Modes/Mode.h"
#include "DataStorage/Betmaps.h"

#ifndef __BEATMAPMANAGER_H__
#define __BEATMAPMANAGER_H__

typedef std::vector<Beatmap*>::iterator beatmapIterator;

typedef enum BeatmapFilter {
    FILTER_NONE,
    FILTER_ALPHA_A_E,
    FILTER_ALPHA_F_J,
    FILTER_ALPHA_K_O,
    FILTER_ALPHA_P_T,
    FILTER_ALPHA_U_Z
} BeatmapFilter;

class BeatmapManager
{
	public:
		static Beatmap& Current() { return *mBeatmapCurrent; }
		
		static void BuildCollection();
        static void BuildMap();
        static bool CheckIndex();
        static void Load(std::string &checksum);
        static void Filter(BeatmapFilter f);
		static uint32_t MapCount();
		static uint32_t SongCount();
		
		static std::vector<BeatmapEntry>& Beatmaps() { return mBeatmapsFiltered; }
	
	protected:
        static void Add(const char* map_filename, const char* map_subdir);
		static Beatmap* mBeatmapCurrent;

        static std::vector<BeatmapEntry> mBeatmapsAll;
        static std::vector<BeatmapEntry> mBeatmapsFiltered;
};

#endif


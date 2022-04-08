#pragma once

#include <cstdio>
#include <string>
#include <dirent.h>
#include <cstring>
#include <vector>

#include "Beatmaps/Beatmap.h"
#include "BeatmapElements.h"
#include "DifficultyManager.h"
#include "Modes/Mode.h"
#include "DataStorage/Betmaps.h"
#include "types.h"
#include "Helpers/PreviewBuffer.h"
#include "Helpers/IO.h"

#include "../lib/OsuParser/OsuParser.h"
#include "../lib/osu_sr_calculator_cpp/osu_sr_calculator/osu_sr_calculator.h"

class BeatmapManager
{
	public:
		static Beatmap& Current() { return *mBeatmapCurrent; }
		
		static void BuildCollection();
        static void BuildMap();
        static bool CheckIndex();
        static void Load(std::string &checksum);
        static void Filter(BeatmapFilter f);
		static OOInt MapCount();
		static OOInt SongCount();

        static bool LoadEntryData(const std::string &filename, const std::string &basedir, BeatmapEntry& bm);
		static std::vector<BeatmapEntry>& Beatmaps() { return mBeatmapsFiltered; }
	
	protected:
        static void Add(const char* map_filename, const char* map_subdir);
		static Beatmap* mBeatmapCurrent;

        static std::vector<BeatmapEntry> mBeatmapsAll;
        static std::vector<BeatmapEntry> mBeatmapsFiltered;
};

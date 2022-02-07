#include "SDL.h"
#include <cstdio>

#include "Mode.h"
#include "Beatmaps/BeatmapManager.h"
#include "Graphics/SpriteManager.h"
#include "System/TextManager.h"

#ifndef __SONGSELECT_H__
#define __SONGSELECT_H__

class SongSelect : public Mode
{
	public:
		SongSelect();
		
		void Update();
		void HandleInput();
	
	protected:
		SpriteManager mSpriteManager;
		
		static const int32_t kSongListXOffset = 37;
		static const int32_t kSongListYOffset = 80;
		static const int32_t kSongListSpacing = 203;
		
		int32_t mCurrentX;
        int32_t mDeltaX;
        int32_t mIndex;

        uint32_t mColTarget;
        uint32_t mColCurrent;
        uint32_t mColMax;
        uint32_t mColSpill;
		
		void MoveSongList(int32_t x);
		void MoveSongListLeft();
		void MoveSongListRight();
};

#endif


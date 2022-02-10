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

        static void PageNext() { if (mCurrentPage < (mCountPages - 1)) { mCurrentPage++; shouldHandlePageUpdate = true; } };
        static void PagePrev() { if (mCurrentPage > 0) { mCurrentPage--; shouldHandlePageUpdate = true; } };
	
	protected:
        void UpdateSonglist();
		SpriteManager mSpriteManager;

        int32_t mSongListSize;
        static int32_t mCurrentPage;
        static int32_t mCountPages;
        static bool shouldHandlePageUpdate;
        int32_t mEntriesPerPage = 4;
        int32_t mEntiesDisplayed = 0;

        int32_t mBeatmapSpritesStartIndex = 0;
        int32_t mSpritesPerBeatmapEntry = 5;

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

